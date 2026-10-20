# Builds dowport.exe (incremental, parallel).   build.bat [-Out <folder>] [-Clean]
# Needs a C compiler in the PATH: Zig ("zig cc"), or MinGW-w64 gcc, or clang. Set the environment variable CC to use another one
# (a full command line such as "C:\tools\zig\zig.exe cc").
param([string]$Out = '', [switch]$Clean)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$obj = Join-Path $root 'build\obj'
if ($Out -eq '') { $Out = Join-Path (Split-Path -Parent $root) 'build' }
$exe = Join-Path $Out 'dowport.exe'

# --- compiler
$cc = $env:CC
if (-not $cc) {
    if (Get-Command zig -ErrorAction SilentlyContinue) { $cc = 'zig cc' }
    elseif (Get-Command gcc -ErrorAction SilentlyContinue) { $cc = 'gcc' }
    elseif (Get-Command clang -ErrorAction SilentlyContinue) { $cc = 'clang' }
    else { throw 'No C compiler found. Install Zig (https://ziglang.org/download/), MinGW-w64 or LLVM clang and put it in the PATH, or set CC.' }
}
$ccParts = $cc -split ' ', 2
$ccExe = $ccParts[0]; [string[]]$ccArgs0 = if ($ccParts.Count -gt 1) { $ccParts[1] -split ' ' } else { @() }

if ($Clean -and (Test-Path $obj)) { Remove-Item -Recurse -Force $obj }
New-Item -ItemType Directory -Force $obj | Out-Null

function Q([string]$x) { if ($x -match '\s') { '"' + $x + '"' } else { $x } }
$flags = @('-c', '-O2', '-w', '-DHOST_DIRECT', '-I', "$root\runtime", '-I', "$root\generated", '-I', "$root\host")
$sources = @(Get-ChildItem "$root\generated\*.c") + @(Get-ChildItem "$root\runtime\*.c") + @(Get-ChildItem "$root\host\*.c")
$headers = @(Get-ChildItem "$root\generated\*.h", "$root\runtime\*.h", "$root\host\*.h")
$newest = ($headers | Measure-Object LastWriteTimeUtc -Maximum).Maximum
$t0 = Get-Date

# --- compile what changed, in parallel
$todo = @(); $objs = @()
foreach ($s in $sources) {
    $o = Join-Path $obj ($s.BaseName + '.o'); $objs += $o
    if (-not (Test-Path $o) -or (Get-Item $o).LastWriteTimeUtc -lt $s.LastWriteTimeUtc -or (Get-Item $o).LastWriteTimeUtc -lt $newest) { $todo += , @($s.FullName, $o) }
}
$running = @(); $failed = $false; $maxp = [Environment]::ProcessorCount
function Wait-Some($running) {
    while ($true) {
        $alive = @($running | Where-Object { -not $_.p.HasExited })
        if ($alive.Count -lt $maxp) { break }
        Start-Sleep -Milliseconds 30
    }
}
foreach ($job in $todo) {
    Wait-Some $running
    $a = ($ccArgs0 + $flags + @('-o', $job[1], $job[0])) | ForEach-Object { Q $_ }
    $p = Start-Process -FilePath $ccExe -ArgumentList $a -NoNewWindow -PassThru; $null = $p.Handle
    $running += [pscustomobject]@{ p = $p; src = $job[0] }
}
foreach ($r in $running) { $r.p.WaitForExit(); if ($r.p.ExitCode -ne 0) { Write-Host "FAILED: $($r.src)"; $failed = $true } }
if ($failed) { exit 1 }

# --- link
New-Item -ItemType Directory -Force $Out | Out-Null
$libs = @('-lopengl32', '-lgdi32', '-luser32', '-lwinmm', '-lole32', '-lxinput1_4', '-lmfplat', '-lmfreadwrite', '-lmfuuid', '-lshell32', '-lshlwapi', '-lpropsys', '-ladvapi32', '-loleaut32', '-luuid')
# the icon of the program: host\ico.png is converted to a multi-size .ico (PNG entries, transparency kept) and compiled as a Windows resource: zig cc compiles the .rc itself,
# with MinGW the resource compiler windres is used; without either the program just has no icon
$png = Join-Path $root 'host\ico.png'; $res = @()
if (Test-Path $png) {
    $ico = Join-Path $obj 'dowport.ico'; $rc = Join-Path $obj 'dowport.rc'
    if (-not (Test-Path $ico) -or (Get-Item $ico).LastWriteTimeUtc -lt (Get-Item $png).LastWriteTimeUtc) {
        Add-Type -AssemblyName System.Drawing
        $src = [System.Drawing.Image]::FromFile($png); $sizes = 16, 24, 32, 48, 64, 128, 256; $blobs = @()
        foreach ($sz in $sizes) {
            $bmp = New-Object System.Drawing.Bitmap $sz, $sz, ([System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
            $g = [System.Drawing.Graphics]::FromImage($bmp); $g.Clear([System.Drawing.Color]::Transparent)
            $g.InterpolationMode = if ($sz -ge 32 -and ($src.Width % $sz) -eq 0) { [System.Drawing.Drawing2D.InterpolationMode]::NearestNeighbor } else { [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic }
            $g.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::Half; $g.DrawImage($src, 0, 0, $sz, $sz); $g.Dispose()
            $ms = New-Object System.IO.MemoryStream; $bmp.Save($ms, [System.Drawing.Imaging.ImageFormat]::Png); $blobs += , $ms.ToArray(); $bmp.Dispose(); $ms.Dispose()
        }
        $src.Dispose()
        $fs = [System.IO.File]::Create($ico); $w = New-Object System.IO.BinaryWriter $fs
        $w.Write([uint16]0); $w.Write([uint16]1); $w.Write([uint16]$sizes.Count); $off = 6 + 16 * $sizes.Count
        for ($i = 0; $i -lt $sizes.Count; $i++) { $d = if ($sizes[$i] -ge 256) { 0 } else { $sizes[$i] }; $w.Write([byte]$d); $w.Write([byte]$d); $w.Write([byte]0); $w.Write([byte]0); $w.Write([uint16]1); $w.Write([uint16]32); $w.Write([uint32]$blobs[$i].Length); $w.Write([uint32]$off); $off += $blobs[$i].Length }
        foreach ($bl in $blobs) { $w.Write($bl) }
        $w.Close(); $fs.Close()
    }
    '1 ICON "dowport.ico"' | Set-Content -Path $rc -Encoding ascii
    if ($ccExe -match 'zig') { $res = @($rc) }
    elseif (Get-Command windres -ErrorAction SilentlyContinue) { $ro = Join-Path $obj 'dowport_rc.o'; Push-Location $obj; & windres 'dowport.rc' -O coff -o $ro; Pop-Location; if ($LASTEXITCODE -eq 0) { $res = @($ro) } }
}
$link = ($ccArgs0 + @('-O2', '-o', $exe) + $objs + $res + $libs + @('-mwindows')) | ForEach-Object { Q $_ }
$p = Start-Process -FilePath $ccExe -ArgumentList $link -NoNewWindow -PassThru; $null = $p.Handle; $p.WaitForExit()
if ($p.ExitCode -ne 0) { Write-Host 'link failed'; exit 1 }
Get-ChildItem $Out -Filter 'dispatch*.lib' -ErrorAction SilentlyContinue | Remove-Item -ErrorAction SilentlyContinue
Remove-Item (Join-Path $Out 'dowport.pdb') -ErrorAction SilentlyContinue
Write-Host ("compiled {0} ({1} files) in {2:N1} s" -f $exe, $todo.Count, ((Get-Date) - $t0).TotalSeconds)
