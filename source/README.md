# Dogs of War – native C source (`source/`)

This is the whole program of the Windows port of *Dogs of War* (Atari ST, Pera Putnik's HAGA version): the game's own 68000 code translated to **C**
(one C function per game routine, with the original instruction in a comment, named registers, variables and hardware registers, and `if` / `else` /
`do … while` structures instead of jumps), plus a native Windows host (window, OpenGL, sound, input, menu, graphics packs). A C compiler is all that is needed to build it.

> The game is abandonware. `generated/` is the translation of the game's program into C, and the executable contains it.

## Layout

```
source/
  build.bat, build.ps1   build script (needs a C compiler: Zig, MinGW-w64 gcc or clang)
  generated/             the game: game_XXXX.c (one C function per routine), game.h, globals.h, dispatch.c
  runtime/               the "machine" around the game code
    cpu.h                registers, flags, macros (CALL, RTS, SETW, FL_*, COND, HOOK, IDLE_*...)
    machine.c            memory, STE I/O (video, MFP, YM2149, keyboard), interrupts, context switch (fibers)
  host/                  the Windows program (see below)
```

## Build

1. Install a C compiler and put it in the `PATH`: [Zig](https://ziglang.org/download/) (`zig cc`), MinGW-w64 (`gcc`) or LLVM `clang`. To use another one set `CC`
   (for example `set CC=C:\tools\zig\zig.exe cc`).
2. Run `build.bat` (incremental and parallel). It writes `..\build\dowport.exe`; `build.bat -Out some\folder` builds somewhere else, `-Clean` rebuilds everything.

## Run

Put the file `FILES/DOW` of your original game in a **`data`** folder next to `dowport.exe` (`data/FILES/DOW`; it is the only original file needed). The first start prepares by itself,
in a few seconds, `assets\original\` (the graphics dump, see below). Everything else (options, saves, music,
shaders, graphics packs) is described in the README of the release.

Developer environment variables (only with `DOW_TEST=1`, nothing is saved to `options.json`): `DOW_STATE=file.dows` start from a snapshot, `DOW_SECONDS`, `DOW_SHOTS=dir` +
`DOW_SHOT_EVERY`, `DOW_KEYS="t:scancode,..."`, `DOW_JOYSEQ="t:bits,..."`, `DOW_MSEQ="t:open|close|up|down|left|right|enter,..."`, `DOW_NOINPUT`, `DOW_GRAFICO=pack`,
`DOW_SONIDO=0`. Graphics dump: `DOW_NODUMP=1` (do not make it at start-up), `DOW_DUMP_OUT=dir` (make it there and exit), `DOW_DUMP_SHOTS=dir` (screenshots of the scripted play), `DOW_DUMP_MISSIONS=n`.

## How to read the generated code

```c
/* 0091E0  wait_two_vbl */
void wait_two_vbl(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    do {
        BUDGET_CHECK(0x91E0U);
        /* 0091E0  tst.w  $26e(a5) */
        /* 0091E4  beq  $91ec */
        IDLE_EVERY(0x91E0U);
        if (rd16(V_026E) != 0) {
            wr16(HW_PALETTE, 0);                             /* 0091E6  move.w  #$0,$ff8240.l */
        } else {
            SETW(d1, d1 | (d0 & 0xFFFF));                    /* 0091EC  or.w  d0,d1 */
        }
        /* 0091EE  tst.w  $1df2(a5) */
        /* 0091F2  bpl  $91e0 */
    } while ((int16_t)rd16(V_1DF2) >= 0);
    ...
```

* **One C function = one game routine.** `bsr`/`jsr` are C calls (`CALL(name, return_address)`) and `rts` is `RTS()` (`return`). The return address is also
  pushed on the emulated stack, because the game reads and writes that stack and the RAM must end up identical to the Atari's.
* **Registers**: `d0..d7`, `a0..a6`, `a7` (stack) are 32-bit global variables. A `move.w` only changes the low part: `SETW(d0, value)`; `SETB` for bytes.
  The flags are `fx fn fz fv fc` and are only computed where someone reads them afterwards; most comparisons are merged with their jump: `cmp.w #$fffc,$a(a0)` + `beq` is
  `if (rd16(a0 + 0xA) == 0xFFFC)`.
* **Memory**: `rd8/rd16/rd32(addr)` and `wr8/wr16/wr32(addr, value)` (big-endian, like the 68000). `V_xxxx` are global variables of the low RAM (the game uses `a5`
  as base and it is always 0: `d(a5)` is simply the address `d`), `HW_xxxx` hardware registers (palette, YM2149, MFP, keyboard).
* **Comments**: the original instruction (`/* 00B0C6  tst.w  $26(a6) */`) goes with every line of C; the ones that are merged carry both comments.
* **Jumps**: what could be structured is `if`/`else`/`do…while`/`for(;;)`; the rest are `goto L_xxxxxx`. The labels used only for resuming stay
  at the end of the routine, in `resume_dispatch:` (it allows loading a saved state in the middle of a call, and computed jumps).
* **Yielding control** (`BUDGET_CHECK`, `HOOK`, `IDLE_*`, `SOFT`): points where the game hands control back to the host — for example the VBL wait —; they are in `runtime/cpu.h`.
  The game runs in a Windows **fiber** (`machine.c`), so every routine is a normal C function with its call stack.
* **Non-local returns**: some routines return "higher up" than usual (they discard their return address and do `rts`). Every `CALL` checks that the popped address
  is the expected one and, if not, the calling routine also ends (`CHECK_NONLOCAL`).
* **Patches**: `g_cpu.patch[0]` (immunity: `player_start_death` returns at once) and `g_cpu.patch[1]` (enemy script fix) are alternatives of an instruction.
* **Edit it freely**: the file `generated/` is the source from now on. To name a routine or a variable, rename it there
  (and in `game.h` / `globals.h`).

## Host (`host/`)

| file | role |
|---|---|
| `tos.c`, `hooks.c`, `state.c`, `boot.c` | mini-TOS (GEMDOS/BIOS/XBIOS), VBL scheduler (timer B palette zones, timer C music, timer A samples, keyboard), host hooks, machine snapshots (`.dows`), start-up state (`boot.c`: what the original loader leaves in memory, built from the game files) |
| `scene.c`, `sprites.c`, `level.c` | scene capture (units → sprites decoded from the game's tables and validated against what the game draws), vertical level maps for graphics packs |
| `render.c`, `present.c` | frame composition at the monitor rate (interpolation, enemy filters, scroll fix) |
| `ym.c`, `mixer.c`, `mp3.c`, `audioout.c` | YM2149, 4-bit sample mixer (multichannel + stereo pan), MP3 via Media Foundation (with music packs), waveOut / WASAPI |
| `lang.c` | language files (`data/languages/*.xml`) and `tr(id)`, used by every text of the menus |
| `video.c`, `menu.c`, `input.c`, `cfg.c` | OpenGL window and shaders, GDI options menu, keyboard/XInput with redefinable keys and buttons, `options.json` |
| `gfxpack.c`, `dump.c`, `png.c` | graphics packs, the graphics dump (`assets/original`), PNG reader/writer |
| `ico.png` | the icon of the executable and of its window: the build script turns it into a multi-size `.ico` (transparency kept) and compiles it as a Windows resource |
| `emu.c`, `main.c` | emulation thread (50 VBL/s clock, saves, weapon helpers), window loop |

**Start-up** (`boot.c`): the original loader (`RUNME.TOS` unpacks and runs HAGA, which probes the machine, shows a menu, reads `FILES/DOW`, patches the game and jumps into it)
always leaves the same memory on this machine, and `boot_state_make` builds it: the game's block (60000 bytes of `FILES/DOW` from offset `$5141C`) at `$2C00` and `$81000`,
the few variables that the loader's code (the hook at `$2000`, the HAGA library at `$3F8000`) still holds and the translated code reads (`LOADER_DATA`, TOS 2.06, 4 MB), and the CPU at the game's entry.
No 68000 code is executed from any file (the loader's routines are translated like the game's), so `RUNME.TOS` and `HAGA` are not needed; `FILES/DOW` only provides data.
It was checked against the real loader by playing the intro, the shop and the 12 missions (the graphics dump comes out identical). If the game ever jumps to an address that is not game code the host logs it and goes back to an earlier snapshot.

**Graphics dump** (`dump.c`): when `assets/original/index.json` does not exist the program plays the game by itself (intro → title → map → shop → each of the 12 missions
with scripted joystick and key input, with the immunity patch for the panels), decodes the graphics tables the game leaves in RAM and writes the PNG sheets, the
`*.idx.png` layers and `index.json` (format 2) of the release README. The content hashes in `index.json` are the ones `gfxpack.c` uses to recognise each graphic, so the
layout must not change; its sheets are identical, cell by cell, to the ones the original tool-chain wrote.

## Verification

The translation was checked instruction by instruction and VBL by VBL against Unicorn, an independent 68000 emulator: the 12 missions, the whole 4 MB of RAM, the YM registers and the palette.

---

# Dogs of War – código fuente nativo en C (`source/`)

Este es el programa completo del port para Windows de *Dogs of War* (Atari ST, versión HAGA de Pera Putnik): el código 68000 del propio juego traducido a **C**
(una función C por rutina del juego, con la instrucción original en un comentario, registros, variables y registros de hardware con nombre, y estructuras `if` / `else` /
`do … while` en lugar de saltos), más un programa anfitrión nativo para Windows (ventana, OpenGL, sonido, entrada, menú, packs gráficos). Solo hace falta un compilador de C para construirlo.

> El juego es abandonware. `generated/` es la traducción del programa del juego a C, y el ejecutable la contiene.

## Estructura

```
source/
  build.bat, build.ps1   script de compilación (necesita un compilador de C: Zig, MinGW-w64 gcc o clang)
  generated/             el juego: game_XXXX.c (una función C por rutina), game.h, globals.h, dispatch.c
  runtime/               la «máquina» que rodea al código del juego
    cpu.h                registros, flags, macros (CALL, RTS, SETW, FL_*, COND, HOOK, IDLE_*...)
    machine.c            memoria, E/S del STE (vídeo, MFP, YM2149, teclado), interrupciones, cambio de contexto (fibras)
  host/                  el programa para Windows (véase más abajo)
```

## Compilación

1. Instala un compilador de C y ponlo en el `PATH`: [Zig](https://ziglang.org/download/) (`zig cc`), MinGW-w64 (`gcc`) o LLVM `clang`. Para usar otro define `CC`
   (por ejemplo `set CC=C:\tools\zig\zig.exe cc`).
2. Ejecuta `build.bat` (incremental y en paralelo). Escribe `..\build\dowport.exe`; `build.bat -Out alguna\carpeta` compila en otro sitio, `-Clean` reconstruye todo.

## Ejecución

Pon el fichero `FILES/DOW` de tu juego original en una carpeta **`data`** junto a `dowport.exe` (`data/FILES/DOW`; es el único fichero original que hace falta). El primer arranque prepara por sí solo,
en unos segundos, `assets\original\` (el volcado de gráficos, véase más abajo). Todo lo demás (opciones, partidas guardadas, música,
shaders, packs gráficos) está descrito en el README de la versión publicada.

Variables de entorno para desarrollo (solo con `DOW_TEST=1`, no se guarda nada en `options.json`): `DOW_STATE=fichero.dows` arranca desde una instantánea, `DOW_SECONDS`, `DOW_SHOTS=dir` +
`DOW_SHOT_EVERY`, `DOW_KEYS="t:scancode,..."`, `DOW_JOYSEQ="t:bits,..."`, `DOW_MSEQ="t:open|close|up|down|left|right|enter,..."`, `DOW_NOINPUT`, `DOW_GRAFICO=pack`,
`DOW_SONIDO=0`. Volcado de gráficos: `DOW_NODUMP=1` (no hacerlo al arrancar), `DOW_DUMP_OUT=dir` (hacerlo allí y salir), `DOW_DUMP_SHOTS=dir` (capturas de la partida guionizada), `DOW_DUMP_MISSIONS=n`.

## Cómo leer el código generado

```c
/* 0091E0  wait_two_vbl */
void wait_two_vbl(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    do {
        BUDGET_CHECK(0x91E0U);
        /* 0091E0  tst.w  $26e(a5) */
        /* 0091E4  beq  $91ec */
        IDLE_EVERY(0x91E0U);
        if (rd16(V_026E) != 0) {
            wr16(HW_PALETTE, 0);                             /* 0091E6  move.w  #$0,$ff8240.l */
        } else {
            SETW(d1, d1 | (d0 & 0xFFFF));                    /* 0091EC  or.w  d0,d1 */
        }
        /* 0091EE  tst.w  $1df2(a5) */
        /* 0091F2  bpl  $91e0 */
    } while ((int16_t)rd16(V_1DF2) >= 0);
    ...
```

* **Una función C = una rutina del juego.** `bsr`/`jsr` son llamadas C (`CALL(nombre, dirección_de_retorno)`) y `rts` es `RTS()` (`return`). La dirección de retorno también se
  apila en la pila emulada, porque el juego lee y escribe esa pila y la RAM debe acabar idéntica a la del Atari.
* **Registros**: `d0..d7`, `a0..a6`, `a7` (pila) son variables globales de 32 bits. Un `move.w` solo cambia la parte baja: `SETW(d0, valor)`; `SETB` para bytes.
  Los flags son `fx fn fz fv fc` y solo se calculan donde alguien los lee después; la mayoría de las comparaciones se funden con su salto: `cmp.w #$fffc,$a(a0)` + `beq` es
  `if (rd16(a0 + 0xA) == 0xFFFC)`.
* **Memoria**: `rd8/rd16/rd32(dir)` y `wr8/wr16/wr32(dir, valor)` (big-endian, como el 68000). `V_xxxx` son variables globales de la RAM baja (el juego usa `a5`
  como base y siempre vale 0: `d(a5)` es simplemente la dirección `d`), `HW_xxxx` son registros de hardware (paleta, YM2149, MFP, teclado).
* **Comentarios**: la instrucción original (`/* 00B0C6  tst.w  $26(a6) */`) acompaña a cada línea de C; las que se funden llevan los dos comentarios.
* **Saltos**: lo que se pudo estructurar son `if`/`else`/`do…while`/`for(;;)`; el resto son `goto L_xxxxxx`. Las etiquetas que solo se usan para reanudar quedan
  al final de la rutina, en `resume_dispatch:` (permite cargar un estado guardado en mitad de una llamada, y los saltos calculados).
* **Ceder el control** (`BUDGET_CHECK`, `HOOK`, `IDLE_*`, `SOFT`): puntos donde el juego devuelve el control al anfitrión —por ejemplo la espera del VBL—; están en `runtime/cpu.h`.
  El juego se ejecuta en una **fibra** de Windows (`machine.c`), así que cada rutina es una función C normal con su pila de llamadas.
* **Retornos no locales**: algunas rutinas vuelven «más arriba» de lo normal (descartan su dirección de retorno y hacen `rts`). Cada `CALL` comprueba que la dirección desapilada
  sea la esperada y, si no lo es, la rutina que llama también termina (`CHECK_NONLOCAL`).
* **Parches**: `g_cpu.patch[0]` (inmunidad: `player_start_death` vuelve de inmediato) y `g_cpu.patch[1]` (corrección del script de enemigos) son alternativas de una instrucción.
* **Edítalo con libertad**: los ficheros de `generated/` son el código fuente de ahora en adelante. Para dar nombre a una rutina o a una variable, renómbrala ahí
  (y en `game.h` / `globals.h`).

## Anfitrión (`host/`)

| fichero | función |
|---|---|
| `tos.c`, `hooks.c`, `state.c`, `boot.c` | mini-TOS (GEMDOS/BIOS/XBIOS), planificador del VBL (zonas de paleta del timer B, música del timer C, muestras del timer A, teclado), ganchos del anfitrión, instantáneas de la máquina (`.dows`), estado de arranque (`boot.c`: lo que el cargador original deja en memoria, construido a partir de los ficheros del juego) |
| `scene.c`, `sprites.c`, `level.c` | captura de escena (unidades → sprites decodificados de las tablas del juego y validados con lo que el juego dibuja), mapas verticales de los niveles para los packs gráficos |
| `render.c`, `present.c` | composición de fotogramas a la frecuencia del monitor (interpolación, filtros de enemigos, corrección del scroll) |
| `ym.c`, `mixer.c`, `mp3.c`, `audioout.c` | YM2149, mezclador de muestras de 4 bits (multicanal + panorámica estéreo), MP3 mediante Media Foundation (con bancos de música), waveOut / WASAPI |
| `lang.c` | ficheros de idioma (`data/languages/*.xml`) y `tr(id)`, usados por todos los textos de los menús |
| `video.c`, `menu.c`, `input.c`, `cfg.c` | ventana OpenGL y shaders, menú de opciones GDI, teclado/XInput con teclas y botones redefinibles, `options.json` |
| `gfxpack.c`, `dump.c`, `png.c` | packs gráficos, el volcado de gráficos (`assets/original`), lector/escritor de PNG |
| `ico.png` | el icono del ejecutable y de su ventana: el script de compilación lo convierte en un `.ico` de varios tamaños (conservando la transparencia) y lo compila como recurso de Windows |
| `emu.c`, `main.c` | hilo de emulación (reloj de 50 VBL/s, guardados, ayudas de armas), bucle de la ventana |

**Arranque** (`boot.c`): el cargador original (`RUNME.TOS` descomprime y ejecuta HAGA, que sondea la máquina, muestra un menú, lee `FILES/DOW`, parchea el juego y salta a él)
deja siempre la misma memoria en esta máquina, y `boot_state_make` la construye: el bloque del juego (60000 bytes de `FILES/DOW` desde el desplazamiento `$5141C`) en `$2C00` y `$81000`,
las pocas variables que conserva el código del cargador (el gancho de `$2000`, la biblioteca HAGA de `$3F8000`) y que lee el código traducido (`LOADER_DATA`, TOS 2.06, 4 MB), y la CPU en el punto de entrada del juego.
No se ejecuta código 68000 de ningún fichero (las rutinas del cargador están traducidas igual que las del juego), así que `RUNME.TOS` y `HAGA` no hacen falta; `FILES/DOW` solo aporta datos.
Se comprobó contra el cargador real jugando la intro, la tienda y las 12 misiones (el volcado de gráficos sale idéntico). Si alguna vez el juego salta a una dirección que no es código del juego, el anfitrión lo registra y vuelve a una instantánea anterior.

**Volcado de gráficos** (`dump.c`): cuando no existe `assets/original/index.json` el programa juega al juego por sí mismo (intro → título → mapa → tienda → cada una de las 12 misiones
con entrada guionizada de joystick y teclado, con el parche de inmunidad para los paneles), decodifica las tablas de gráficos que el juego deja en la RAM y escribe las hojas PNG, las
capas `*.idx.png` y el `index.json` (formato 2) del README de la versión publicada. Los hashes de contenido de `index.json` son los que usa `gfxpack.c` para reconocer cada gráfico, así que la
disposición no debe cambiar; sus hojas son idénticas, celda por celda, a las que escribía la cadena de herramientas original.

## Verificación

La traducción se comprobó instrucción por instrucción y VBL por VBL contra Unicorn, un emulador 68000 independiente: las 12 misiones, los 4 MB de RAM completos, los registros del YM y la paleta.
