# Changelog – Dogs of War, Windows port at 60 fps

This file lists, in order, the steps that led to the current version. Dates are those of the work sessions (2026). The project started as an analysis of the original game and went through three technical stages before reaching its present form: a prototype that ran the game on a 68000 emulator library inside a Python program, a hybrid with a native core, and the current **all-C port in which the whole game is translated to C**. Releases are numbered: **1.0** (2026-10-19) was the first public one and **1.1** (2026-10-20) the current one; the steps before 1.0 are listed by date.

## Version 1.1 (2026-10-20) – twin-stick control, interactive gamepad picture, resolution-independent CRT mask
Main feature, switchable in **Controls → Twin-stick: right stick aims** (option `pad_dual_stick`, on by default):
- **Left stick** moves the soldier as always. **Right stick** (8 directions, 45-degree sectors, dead zone 40 %) sets the direction he **faces and shoots**, whatever way the left stick moves him: you can walk up while firing to the right, or strafe and shoot diagonally. When the right stick is released the soldier goes back to facing the way he walks.
- While the right stick is pushed, the original "fire without a direction = stand still" lock is released, so the left stick can start moving him while he keeps shooting.
- **Right stick also fires** (option `pad_aim_autofire`, on by default): the fire button is held while the right stick is pushed. Off: aim with the right stick, fire with the usual button. It only acts during a mission (menus and the shop ignore the right stick).
- How it works: the game keeps the facing / fire direction in one byte of the player structure (`$36`); only that byte is taken from the right stick (`host_aim()` called from the direction routine `$9CAC`), the movement flags are untouched. Player 1 uses pad 1, player 2 pad 2.
- The picture of the gamepad in the Controls tab is now drawn by the program from your illustration (`padmap.c`) instead of being a fixed PNG, without the title boxes: every box shows what that button does **now**, so it follows the buttons redefined in the Controls window, the twin-stick option (the right stick is greyed out when it is off), an action assigned to L3 / R3 (shown under its stick) and the menu language. The texts are the `padmap.*` entries of the language files. `assets\controller*.png` are no longer used.
- The explanation box of a tab with a picture is only as tall as its longest explanation needs, and the picture gets the rest.
- Portability: the picture is plain C (`padmap.c`) that draws through a small interface (`padmap_backend.h`); the Windows implementation is `padmap_gdi.c`. Another system only needs another implementation of those ~10 functions.
- The explanation box of a tab with a picture is measured in pixels (not in lines of the options' size) and uses a smaller font, and the panel takes the whole height: the picture of the pad gets all the rest (about 25 % taller than before). The options window now has the same size on every tab (the Controls tab used to make it taller).
- **Phosphor mask independent of the resolution**: the CRT mask was computed in screen pixels (3 pixels per triad), so on a 4K screen it was half the size relative to the picture and hardly visible. It is now 3 pixels at 1080 lines and grows with the picture height (`mkz` in the built-in CRT shader, `MaskAA` in `fakelottes.glsl` and `fakelottes-geom.glsl`, the same factor in `crt-pi.glsl` and `fake-CRT-Geom.glsl`; `crt-nes-mini.glsl` has no mask and needed no change); each pixel averages the mask over its area, so intermediate scales (1440p...) have no moire, and at 1080p the result is exactly the old one. Test: `DOW_MASKH=<height>` (with `DOW_TEST=1`) pretends the picture has that height.
- **Keys for typing names**: the high-score name entry ignored some keys because the keyboard table lacked **V** and **Z** (Z was also swallowed as the second fire key), Backspace, Delete and the punctuation keys. They are now passed to the game (Z only on the screens that take typing, so it still works as fire in the mission).
- Development helpers (only with `DOW_TEST=1`): `DOW_AIM=<J_* bits>` + `DOW_AIM_FROM` / `DOW_AIM_TO` push the right stick by script; `DOW_AUTOMISSION=<mission value>` goes from the map into that mission by itself and plays a fixed left/right-stick script, to read the facing byte in the `DOW_PLAYERLOG` log (`face36`).

## Version 1.0 (2026-10-19) – all C
The program is a single native C executable. Nothing is interpreted and nothing needs Python.

**Start-up and files**
- The only original file needed is `data\FILES\DOW`, and it only provides data. `RUNME.TOS` and `HAGA` (the original loader) are no longer read: all their code is translated to C, and the few variables of the loader that the code reads are kept in the program.
- The memory the original loader leaves when it starts the game is built directly (`boot.c`); no 68000 interpreter, no start-up cache and no snapshot of the loader exist any more. If the game ever jumps to an address that is not translated code, the program logs it and goes back to a snapshot of a few seconds earlier.
- The graphics dump (`assets\original`) is made by the program itself on the first start, playing the game with scripted input.
- The loader's text screen (copyright) fades in, stays for 3 s (`loading_seconds`), fades out to black and, after one second of black, the intro follows. The original sequence (text at once, fade to grey, fade back, clear, a black wait of several seconds) was replaced because the disk read it was timed on is instant here.

**Menu**
- The options are grouped in tabs (Game, Sound, Video, Motion, Controls, System). L1/R1 or Page Up/Page Down change tab, and the menu reopens on the last tab and option used.
- The Controls tab shows a picture of the gamepad (`assets\controller.png`, with its transparency respected) and an option that opens a window to redefine the keys and the pad buttons.
- Fixed the gamepad picture not showing when the program sits in a folder with a long path.
- Menu texts come from XML language files (`data\languages`): English and Spanish, selectable in System → Language; other languages can be added by copying a file.
- The explanation of each option appears at the bottom in a smaller font than the options; for options with several values, a general explanation is followed by a line and the explanation of the chosen value (only this second text changes its font size to fit).
- Texts of the options were reviewed and shortened.

**Video**
- New *Shader source* option (scaled, 320x200 averaged, 320x200 exact) that decides what the shaders start from; external shaders always see a 320x200 screen.
- Fixed a black screen at start-up with the "None" effect.
- The darkening at the end of a mission (the game subtracts 1 from every component of its 512-colour palette, which drifts the hues in big steps) is now a smooth, lighter dimming that keeps the original colours at full 8-bit precision.
- The documentation states that the supplied external shaders come from the RetroArch / libretro collection and keep their own licences.

**Sound**
- MP3 songs can have a text file with loop points (`Loop point start` / `Loop point back`), counted in samples at the sample rate of the file itself (44100 or 48000 Hz).
- The MP3 music keeps playing while the options menu is open, and the *mission failed* screen waits for its song to end or for a button.
- The mouse pointer is never shown over the game.
- The songs that end by themselves (end of level, bonus, game over) can be skipped: pressing fire or a key fades the MP3 out (0.7 s) and the game goes on, instead of waiting for the whole song.
- The volumes of the chip music and chip effects are now real gains applied while mixing, instead of the 16 steps of the sound chip.

**Project**
- The executable and its window have an icon (the soldier of `ico.png`, with its transparency, in several sizes).
- The Python port was abandoned (it meant double work) and the program no longer uses Python for anything; its tools were archived outside the project.
- README rewritten and organised, in English and Spanish; this changelog; a README for the source code.
- Folders: `source\` (all the C source), `build\` (the runnable program). The repository keeps only what is needed to build and run.

## 2026-10-14 – Preparing the first release (still two cores: C and Python)
- Controls redefinable (keys and gamepad buttons), with a *Restore default options* action that reloads `default.json`.
- *Music pack* option: each subfolder of `music\` is a bank of MP3 songs.
- Frame interpolation can be turned off; *Game speed* option that matches the game's logic rate to the screen refresh; pad shortcuts (L2 hold position, R1/L1 next/previous weapon).
- Latency options: fast input polling (1000 Hz thread), low-latency audio output (WASAPI) with a fine sync trim; default emulation margin set to 3.6 frames after measuring that lower values make the scroll bounce when it stops.
- Tried and removed: pixel-aligned background and a low-margin switch (they looked worse than the default).
- Faster composition of full-screen graphic layers (12 ms → 2.4 ms); protection masks of the graphics dump refined.
- Removed from the program: video capture, test keys, the Spanish command-line options and the folder selector (the original files go next to the executable). All source comments and logs translated to English.
- Fixed Alt+F4; external shaders grey out the built-in effect sliders.

## 2026-10-13 – Native C program (the game translated to C)
- **The whole 68000 program of the game was translated to C** (one C function per game routine, with the original instruction in a comment), running inside a small STE machine (mini-TOS, MFP timers, keyboard, YM2149, video) written in C. A native Windows host (OpenGL video, mixer, MP3 through Windows Media Foundation, waveOut/WASAPI audio, XInput, GDI menu, graphics packs) replaced the Python one. The translation was checked against the independent emulator, instruction by instruction, VBL by VBL over the 12 missions (4 MB of RAM, sound registers and palette identical).
- Canonical graphics dump and graphics packs: every sprite, tile and fixed screen of the game is exported to PNG sheets with an index; a pack of replacements is recognised by content and drawn instead, with the game's animation and the 60 fps interpolation. Screen layers, masks and occluders handle the parts the game still draws itself.
- MP3 music per song, with the songs that end by themselves played once and whole and the looping ones without a gap.
- A scroll inertia filter was tried and removed (it showed repeated rows at the edge).
- Explosions in horizontal-scroll levels fixed **in the game** (the scroll step forgot the explosion list), mines/turrets and explosions made smooth.
- Fixed a crash in Italy (a scroll hook added the scroll to one more list entry than exists) and the game closing when rescuing a hostage (`MOVEP` instruction).
- Video capture (later removed) used to analyse problems; full-screen presentation without tearing; low CPU use (about 28 % of one core at 60 fps).
- Explosions drawn on 16-pixel steps made smooth; "bad sprites" at the screen edge fixed.

## 2026-10-12 – From prototype to playable program
- **Real-time engine** with an OpenGL output (built-in effects: scanlines, soft/full CRT; aspect modes; filters), external RetroArch-style `.glsl` shaders, an options menu and saved options.
- **Saving**: the game has no saves, so full snapshots are taken. The save always stores the mission map before the current mission (with money and missions done); *Back to the mission map* does the same without a slot.
- **Sound**: the sound driver was analysed; a channel that disappeared while a sample played was fixed; the title music is started when the intro is skipped; MP3 songs per game song; a multichannel mixer for sampled effects and stereo panning by on-screen position.
- **Pad**: A fire, B quick rocket launcher, X quick grenade, L1/R1 weapon back/forward, Start for the menu, Back to leave the mission; weapon "previous" implemented by reading the player's structure.
- Menu and documentation in English; credits line.
- Fixed pauses of the emulation in the game's waiting loops.
- Automatic full game played for tests (an auto-pilot) and graphic sheets cleaned.

## 2026-10-11 – Analysis, first prototype and smoothing
- **Decision**: keep the game's logic at 25 Hz untouched and **interpolate only the drawing** to 60 fps, instead of patching the game to 50 fps. The result must be a native Windows executable, with graphic and sound resources in separate files.
- Analysis with the Hatari emulator (reference): the game only advances beyond the title screen when started from a hard-disk image; a scripted, deterministic test harness was built; static disassembly of the game (about 13,000 instructions); the main loop, the VBL wait, the sprite and scroll routines were located.
- Own 68000 core (Python + Unicorn), own mini-TOS able to run the original loader (`RUNME.TOS`/`HAGA`) and start the game without Hatari.
- **Scene capture**: for every logic frame the background, the HUD and each drawn object are recorded, and drawn at any intermediate instant: **60 fps interpolation demonstrated**. First Windows executable (Python, PyInstaller); real-time engine with keyboard and joystick input.
- Fixes found on the way: the TOS variables that the game uses as its own data (`$462`, `$466`, `$4BA`) were being incremented by the host (corrupted enemies); key presses released in the same VBL were never seen by the game; music too fast (the game programs MFP timer C at ≈48.6 Hz, not 200 Hz).
- Smoothing of scroll and sprites: sub-pixel scroll, display clock tied to the screen refresh, enemy motion filters, handling of the horizontal-scroll levels, and no catching-up after a long pause of the emulation (which made the scroll jump at each level section).
- **Fix of the original game**: the routine that runs each enemy's script (`$B0B8`) returns without moving the enemy on the frame it loads a new order, which causes micro-stutters (the original Atari version and Hatari do it too). A one-word patch (`RTS` → `BRA.B`) fixes it; it is on by default and can be switched off.
- YM2149 chip synthesis from the game's register writes with their time stamps, XInput pads, replaceable graphics (first version, later redesigned), immunity and infinite-money keys for testing (later removed from the keys).


---

# Registro de cambios – Dogs of War, port para Windows a 60 fps

Este fichero enumera, por orden, los pasos que llevaron a la versión actual. Las fechas son las de las sesiones de trabajo (2026). El proyecto empezó como un análisis del juego original y pasó por tres etapas técnicas hasta su forma actual: un prototipo que ejecutaba el juego con una biblioteca de emulación 68000 dentro de un programa en Python, un híbrido con núcleo nativo y el **port actual, todo en C, en el que el juego completo está traducido a C**. Las versiones publicadas están numeradas: la **1.0** (2026-10-19) fue la primera pública y la **1.1** (2026-10-20) es la actual; los pasos anteriores a la 1.0 se listan por fecha.

## Versión 1.1 (2026-10-20) – control de doble stick, imagen interactiva del mando y máscara CRT independiente de la resolución
Función principal, que se activa y desactiva en **Controles → Doble stick: el derecho apunta** (opción `pad_dual_stick`, activada por defecto):
- El **stick izquierdo** mueve al soldado como siempre. El **stick derecho** (8 direcciones, sectores de 45°, zona muerta del 40 %) fija la dirección en que **mira y dispara**, sea cual sea la dirección en que lo mueva el izquierdo: puedes andar hacia arriba disparando a la derecha, o desplazarte lateralmente disparando en diagonal. Al soltar el derecho, el soldado vuelve a mirar hacia donde camina.
- Mientras el derecho está pulsado se suspende el bloqueo original «disparar sin dirección = quedarse quieto», de modo que el izquierdo puede empezar a moverlo sin que deje de disparar.
- **El stick derecho también dispara** (opción `pad_aim_autofire`, activada por defecto): se mantiene el botón de disparo mientras el derecho está pulsado. Desactivada, se apunta con el derecho y se dispara con el botón de siempre. Solo actúa durante una misión (los menús y la tienda ignoran el derecho).
- Cómo funciona: el juego guarda la dirección en que mira y dispara el soldado en un solo byte de la estructura del jugador (`$36`); solo ese byte se toma del stick derecho (`host_aim()`, llamada desde la rutina de dirección `$9CAC`) y las banderas de movimiento no se tocan. El jugador 1 usa el mando 1 y el jugador 2, el mando 2.
- La imagen del mando de la pestaña Controles la dibuja ahora el programa a partir de tu ilustración (`padmap.c`) en lugar de ser un PNG fijo, sin las cajas del título: cada caja muestra qué hace ese botón **ahora**, de modo que sigue los botones redefinidos en la ventana de Controles, la opción de doble stick (el stick derecho aparece atenuado si está desactivada), una acción asignada a L3 / R3 (se muestra bajo su stick) y el idioma del menú. Los textos son las entradas `padmap.*` de los ficheros de idioma. `assets\controller*.png` ya no se usan.
- La caja de explicación de una pestaña con imagen solo tiene la altura que necesita su explicación más larga, y la imagen aprovecha el resto.
- Portabilidad: la imagen es C puro (`padmap.c`) que dibuja a través de una interfaz pequeña (`padmap_backend.h`); la implementación para Windows es `padmap_gdi.c`. Otro sistema solo necesita otra implementación de esas ~10 funciones.
- La caja de explicación de una pestaña con imagen se mide en píxeles (no en líneas del tamaño de las opciones) y usa una fuente menor, y el panel ocupa toda la altura: la imagen del mando se queda con todo el resto (un 25 % más alta que antes). La ventana de opciones tiene ahora el mismo tamaño en todas las pestañas (la de Controles la hacía más alta).
- **Máscara de fósforo independiente de la resolución**: la máscara CRT se calculaba en píxeles de pantalla (3 píxeles por tríada), así que en una pantalla 4K era la mitad de grande respecto a la imagen y apenas se apreciaba. Ahora mide 3 píxeles con 1080 líneas y crece con la altura de la imagen (`mkz` en el shader CRT incorporado, `MaskAA` en `fakelottes.glsl` y `fakelottes-geom.glsl`, el mismo factor en `crt-pi.glsl` y `fake-CRT-Geom.glsl`; `crt-nes-mini.glsl` no tiene máscara y no necesitó cambios); cada píxel promedia la máscara sobre su área, así que las escalas intermedias (1440p...) no producen muaré, y a 1080p el resultado es exactamente el de antes. Prueba: `DOW_MASKH=<altura>` (con `DOW_TEST=1`) simula que la imagen tiene esa altura.
- **Teclas para escribir nombres**: la entrada del nombre en los récords ignoraba algunas teclas porque la tabla del teclado no tenía la **V** ni la **Z** (la Z además se la tragaba como segundo botón de disparo), Retroceso, Supr ni las teclas de puntuación. Ahora se pasan al juego (la Z solo en las pantallas que piden texto, así que sigue funcionando como disparo en la misión).
- Ayudas de desarrollo (solo con `DOW_TEST=1`): `DOW_AIM=<bits J_*>` + `DOW_AIM_FROM` / `DOW_AIM_TO` pulsan el stick derecho por guion; `DOW_AUTOMISSION=<valor de la misión>` va del mapa a esa misión por sí solo y reproduce un guion fijo de stick izquierdo y derecho, para leer el byte de dirección en el registro de `DOW_PLAYERLOG` (`face36`).

## Versión 1.0 (2026-10-19) – todo en C
El programa es un único ejecutable nativo en C. No se interpreta nada y no se necesita Python.

**Arranque y ficheros**
- El único fichero original necesario es `data\FILES\DOW`, y solo aporta datos. `RUNME.TOS` y `HAGA` (el cargador original) ya no se leen: todo su código está traducido a C, y las pocas variables del cargador que lee el código se conservan en el programa.
- La memoria que deja el cargador original al iniciar el juego se construye directamente (`boot.c`); ya no existen ni intérprete 68000, ni caché de arranque, ni instantánea del cargador. Si el juego salta alguna vez a una dirección que no es código traducido, el programa lo registra y vuelve a una instantánea de unos segundos antes.
- El volcado de gráficos (`assets\original`) lo hace el propio programa en el primer arranque, jugando al juego con entradas guionizadas.
- La pantalla de texto del cargador (copyright) aparece con un fundido, permanece 3 s (`loading_seconds`), se desvanece a negro y, tras un segundo en negro, sigue la intro. Se sustituyó la secuencia original (texto de golpe, fundido a gris, fundido de vuelta, borrado y una espera en negro de varios segundos) porque la lectura de disco en la que se basaba su duración aquí es instantánea.

**Menú**
- Las opciones se agrupan en pestañas (Juego, Sonido, Vídeo, Movimiento, Controles, Sistema). L1/R1 o Re Pág/Av Pág cambian de pestaña, y el menú se reabre en la última pestaña y opción usadas.
- La pestaña Controles muestra una imagen del mando (`assets\controller.png`, respetando su transparencia) y una opción que abre una ventana para redefinir las teclas y los botones del mando.
- Corregida la imagen del mando, que no se mostraba cuando el programa estaba en una carpeta con una ruta larga.
- Los textos del menú salen de ficheros de idioma XML (`data\languages`): inglés y español, seleccionables en Sistema → Idioma; se pueden añadir otros idiomas copiando un fichero.
- La explicación de cada opción aparece abajo con una fuente más pequeña que la de las opciones; en las opciones con varios valores, una explicación general va seguida de una línea y de la explicación del valor elegido (solo este segundo texto cambia de tamaño de fuente para caber).
- Se revisaron y acortaron los textos de las opciones.

**Vídeo**
- Nueva opción *Origen del shader* (escalado, 320x200 promediado, 320x200 exacto) que decide de qué parten los shaders; los shaders externos ven siempre una pantalla de 320x200.
- Corregida una pantalla negra al arrancar con el efecto «Ninguno».
- El oscurecimiento al terminar una misión (el juego resta 1 a cada componente de su paleta de 512 colores, lo que desvía los tonos en pasos grandes) es ahora un oscurecimiento suave y más ligero que conserva los colores originales a 8 bits de precisión.
- La documentación indica que los shaders externos incluidos proceden de la colección de RetroArch / libretro y conservan sus propias licencias.

**Sonido**
- Las canciones MP3 pueden tener un fichero de texto con puntos de bucle (`Loop point start` / `Loop point back`), contados en muestras a la frecuencia de muestreo del propio fichero (44100 o 48000 Hz).
- La música MP3 sigue sonando mientras el menú de opciones está abierto, y la pantalla de *misión fallida* espera a que termine su canción o a que se pulse un botón.
- El puntero del ratón no se muestra nunca sobre el juego.
- Las canciones que terminan solas (fin de nivel, bonus, fin de partida) se pueden saltar: al pulsar fuego o una tecla el MP3 se apaga con un fundido (0,7 s) y el juego continúa, en lugar de esperar a toda la canción.
- Los volúmenes de la música y de los efectos del chip son ahora ganancias reales aplicadas al mezclar, en lugar de los 16 pasos del chip de sonido.

**Proyecto**
- El ejecutable y su ventana tienen un icono (el soldado de `ico.png`, con su transparencia, en varios tamaños).
- El port en Python se abandonó (suponía doble trabajo) y el programa ya no usa Python para nada; sus herramientas se archivaron fuera del proyecto.
- README reescrito y ordenado, en inglés y en español; este registro de cambios; un README del código fuente.
- Carpetas: `source\` (todo el código fuente en C), `build\` (el programa ejecutable). El repositorio solo conserva lo necesario para compilar y ejecutar.

## 2026-10-14 – Preparación de la primera versión (todavía con dos núcleos: C y Python)
- Controles redefinibles (teclas y botones del mando), con una acción *Restaurar opciones por defecto* que recarga `default.json`.
- Opción *Banco de música*: cada subcarpeta de `music\` es un banco de canciones MP3.
- La interpolación de fotogramas se puede desactivar; opción *Velocidad del juego* que iguala el ritmo lógico del juego al refresco de pantalla; atajos del mando (L2 mantener posición, R1/L1 arma siguiente/anterior).
- Opciones de latencia: lectura rápida de entrada (hilo a 1000 Hz), salida de audio de baja latencia (WASAPI) con ajuste fino de sincronía; margen de emulación por defecto fijado en 3,6 fotogramas tras medir que valores menores hacen que el scroll rebote al parar.
- Probado y retirado: fondo alineado a la rejilla y un interruptor de margen bajo (se veían peor que el valor por defecto).
- Composición más rápida de las capas gráficas de pantalla completa (12 ms → 2,4 ms); máscaras de protección del volcado de gráficos refinadas.
- Quitado del programa: captura de vídeo, teclas de prueba, las opciones de línea de comandos en español y el selector de carpeta (los ficheros originales van junto al ejecutable). Todos los comentarios y registros del código se tradujeron al inglés.
- Corregido Alt+F4; los shaders externos atenúan los deslizadores de efectos incorporados.

## 2026-10-13 – Programa nativo en C (el juego traducido a C)
- **Todo el programa 68000 del juego se tradujo a C** (una función C por rutina del juego, con la instrucción original en un comentario), y se ejecuta dentro de una pequeña máquina STE (mini-TOS, timers del MFP, teclado, YM2149, vídeo) escrita en C. Un anfitrión nativo de Windows (vídeo OpenGL, mezclador, MP3 mediante Windows Media Foundation, audio waveOut/WASAPI, XInput, menú GDI, packs gráficos) sustituyó al de Python. La traducción se comprobó contra el emulador independiente, instrucción por instrucción y VBL por VBL en las 12 misiones (4 MB de RAM, registros de sonido y paleta idénticos).
- Volcado canónico de gráficos y packs gráficos: cada sprite, tile y pantalla fija del juego se exporta a hojas PNG con un índice; un pack de reemplazos se reconoce por su contenido y se dibuja en su lugar, con la animación del juego y la interpolación a 60 fps. Las capas de pantalla, las máscaras y los oclusores gestionan lo que el juego sigue dibujando por sí mismo.
- Música MP3 por canción, con las canciones que terminan solas reproducidas una vez y completas, y las que hacen bucle sin cortes.
- Se probó y se retiró un filtro de inercia del scroll (mostraba filas repetidas en el borde).
- Explosiones en niveles de scroll horizontal corregidas **en el juego** (el paso del scroll olvidaba la lista de explosiones); minas, torretas y explosiones suavizadas.
- Corregido un cuelgue en Italia (un gancho del scroll sumaba el desplazamiento a una entrada más de las que tiene la lista) y el cierre del juego al rescatar a un rehén (instrucción `MOVEP`).
- Captura de vídeo (retirada después) usada para analizar problemas; presentación a pantalla completa sin tearing; bajo uso de CPU (en torno al 28 % de un núcleo a 60 fps).
- Explosiones dibujadas en pasos de 16 píxeles suavizadas; «sprites malos» en el borde de la pantalla corregidos.

## 2026-10-12 – De prototipo a programa jugable
- **Motor en tiempo real** con salida OpenGL (efectos incorporados: líneas de escaneo, CRT suave/completo; modos de aspecto; filtros), shaders `.glsl` externos al estilo RetroArch, un menú de opciones y opciones guardadas.
- **Guardado**: el juego no tiene partidas guardadas, así que se toman instantáneas completas. El guardado almacena siempre el mapa de misiones anterior a la misión actual (con el dinero y las misiones hechas); *Volver al mapa de misiones* hace lo mismo sin ranura.
- **Sonido**: se analizó el controlador de sonido; se corrigió un canal que desaparecía mientras sonaba una muestra; la música del título se inicia cuando se salta la intro; canciones MP3 por canción del juego; un mezclador multicanal para los efectos muestreados y panorámica estéreo según la posición en pantalla.
- **Mando**: A dispara, B lanzacohetes rápido, X granada rápida, L1/R1 arma atrás/adelante, Start para el menú, Back para abandonar la misión; el arma «anterior» se implementó leyendo la estructura del jugador.
- Menú y documentación en inglés; línea de créditos.
- Corregidas las paradas de la emulación en los bucles de espera del juego.
- Se jugó una partida completa automática para pruebas (un piloto automático) y se limpiaron las hojas gráficas.

## 2026-10-11 – Análisis, primer prototipo y suavizado
- **Decisión**: mantener intacta la lógica del juego a 25 Hz e **interpolar solo el dibujado** a 60 fps, en lugar de parchear el juego a 50 fps. El resultado debe ser un ejecutable nativo de Windows, con los recursos gráficos y sonoros en ficheros aparte.
- Análisis con el emulador Hatari (referencia): el juego solo avanza más allá de la pantalla de título si se arranca desde una imagen de disco duro; se construyó un banco de pruebas determinista con entradas guionizadas; desensamblado estático del juego (unas 13.000 instrucciones); se localizaron el bucle principal, la espera de VBL y las rutinas de sprites y scroll.
- Núcleo 68000 propio (Python + Unicorn), mini-TOS propio capaz de ejecutar el cargador original (`RUNME.TOS`/`HAGA`) y arrancar el juego sin Hatari.
- **Captura de escena**: por cada fotograma lógico se registran el fondo, el HUD y cada objeto dibujado, y se dibujan en cualquier instante intermedio: **interpolación a 60 fps demostrada**. Primer ejecutable para Windows (Python, PyInstaller); motor en tiempo real con entrada de teclado y joystick.
- Fallos encontrados por el camino: las variables del TOS que el juego usa como datos propios (`$462`, `$466`, `$4BA`) las incrementaba el anfitrión (enemigos corruptos); las pulsaciones soltadas en el mismo VBL nunca las veía el juego; música demasiado rápida (el juego programa el timer C del MFP a ≈48,6 Hz, no a 200 Hz).
- Suavizado del scroll y de los sprites: scroll subpíxel, reloj de presentación atado al refresco de pantalla, filtros del movimiento de los enemigos, tratamiento de los niveles de scroll horizontal y sin recuperar el tiempo tras una parada larga de la emulación (lo que hacía saltar el scroll en cada tramo del nivel).
- **Corrección del juego original**: la rutina que ejecuta el guion de cada enemigo (`$B0B8`) vuelve sin mover al enemigo en el fotograma en que carga una orden nueva, lo que provoca microtirones (la versión original de Atari y Hatari también lo hacen). Un parche de una palabra (`RTS` → `BRA.B`) lo corrige; viene activado por defecto y se puede desactivar.
- Síntesis del chip YM2149 a partir de las escrituras de registros del juego con su marca de tiempo, mandos XInput, gráficos reemplazables (primera versión, rediseñada después), teclas de inmunidad y dinero infinito para pruebas (retiradas después de las teclas).
