/*
 * video.c - Windows window + OpenGL: RGB texture at scale S, scaling with aspect and filter, built-in
 * CRT shaders (none, scanlines, crt) and external RetroArch ones (single-pass .glsl), and an overlaid RGBA layer (menu) that is drawn AFTER the shader.
 */
#include "video.h"
#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#endif

VidCfg vcfg = { "Soft CRT", 1, 0, "", { 0.35f, 0.25f, 0.10f, 1.10f, 0.35f, 0.15f }, 0 };
const char *VID_FILTERS[3] = { "Sharp", "Smooth", "Sharp bilinear" };
const char *VID_SOURCES[3] = { "Scaled", "320x200 averaged", "320x200 exact" };
const char *VID_ASPECTS[4] = { "Original 4:3", "Square pixels 16:10", "Stretch", "Integer" };
const char *VID_PRESETS[4] = { "None", "Scanlines", "Soft CRT", "Full CRT" };

#ifdef _WIN32
typedef char GLchar_;
#ifndef GL_RGB8
#define GL_RGB8 0x8051
#endif
#ifndef GL_RGBA8
#define GL_RGBA8 0x8058
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4
#define GL_TEXTURE0 0x84C0
#define GLFN(ret, name, args) typedef ret (APIENTRY *PFN_##name) args; static PFN_##name p_##name
GLFN(GLuint, glCreateShader, (GLenum)); GLFN(void, glShaderSource, (GLuint, GLsizei, const GLchar_ **, const GLint *)); GLFN(void, glCompileShader, (GLuint));
GLFN(void, glGetShaderiv, (GLuint, GLenum, GLint *)); GLFN(void, glGetShaderInfoLog, (GLuint, GLsizei, GLsizei *, GLchar_ *)); GLFN(GLuint, glCreateProgram, (void));
GLFN(void, glAttachShader, (GLuint, GLuint)); GLFN(void, glLinkProgram, (GLuint)); GLFN(void, glGetProgramiv, (GLuint, GLenum, GLint *)); GLFN(void, glGetProgramInfoLog, (GLuint, GLsizei, GLsizei *, GLchar_ *));
GLFN(void, glUseProgram, (GLuint)); GLFN(GLint, glGetUniformLocation, (GLuint, const GLchar_ *)); GLFN(GLint, glGetAttribLocation, (GLuint, const GLchar_ *));
GLFN(void, glUniform1i, (GLint, GLint)); GLFN(void, glUniform1f, (GLint, GLfloat)); GLFN(void, glUniform2f, (GLint, GLfloat, GLfloat)); GLFN(void, glUniformMatrix4fv, (GLint, GLsizei, GLboolean, const GLfloat *));
GLFN(void, glGenBuffers, (GLsizei, GLuint *)); GLFN(void, glBindBuffer, (GLenum, GLuint)); GLFN(void, glBufferData, (GLenum, ptrdiff_t, const void *, GLenum));
GLFN(void, glVertexAttribPointer, (GLuint, GLint, GLenum, GLboolean, GLsizei, const void *)); GLFN(void, glEnableVertexAttribArray, (GLuint)); GLFN(void, glDisableVertexAttribArray, (GLuint));
GLFN(void, glActiveTexture, (GLenum)); GLFN(void, glDeleteProgram, (GLuint)); GLFN(void, glDeleteShader, (GLuint));
GLFN(void, glGenVertexArrays, (GLsizei, GLuint *)); GLFN(void, glBindVertexArray, (GLuint));
typedef BOOL (WINAPI *PFN_swapint)(int); static PFN_swapint p_swapint; static long (WINAPI *p_dwmflush)(void);
static int force_vsync = 0, use_dwm = 0; static int fullscreen;
/* Windowed: interval 0 + DwmFlush (waits without using CPU). Fullscreen (DWM composition is skipped, DwmFlush does not synchronize well): the driver's normal vsync. */
static void apply_sync(void) { use_dwm = p_dwmflush && !force_vsync && 1; if (p_swapint) p_swapint(getenv("DOW_SWAPINT") ? atoi(getenv("DOW_SWAPINT")) : (use_dwm ? 0 : 1)); }
#define LOADGL(n) p_##n = (PFN_##n)wglGetProcAddress(#n)

static HWND hwnd; static HDC hdc; static HGLRC hrc; static int cw_ = 0, ch_ = 0;     /* size of the client area */
static int src_w_, src_h_, tex_w_, tex_h_; static GLuint tex_main, tex_ov, vbo, vao;
static int ov_w = 0, ov_h = 0, ov_on = 0; static int fullscreen = 0; static RECT win_rect; static LONG win_style;
static char shaders_dir_[500]; static char ext_names[64][64]; static char ext_paths[64][520]; static int n_ext = 0;
static char err_[400]; static int has_err = 0; static int frame_n = 0; static char shot_path[600]; static char ext_cur[64];

/* ------------------------------------------------------------------------------------------------ eventos */
#define EVQ 512
static VEvent evq_[EVQ]; static int evq_h = 0, evq_n = 0;
static void ev_push(int type, int vk, int down, int mods, int rep) { if (evq_n < EVQ) { VEvent *e = &evq_[(evq_h + evq_n) % EVQ]; e->type = type; e->vk = vk; e->down = down; e->mods = mods; e->rep = rep; evq_n++; } }
static LRESULT CALLBACK wndproc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_CLOSE: ev_push(VE_QUIT, 0, 0, 0, 0); return 0;
    case WM_DESTROY: return 0;
    case WM_ERASEBKGND: return 1;
    case WM_SETCURSOR: if (LOWORD(l) == HTCLIENT) { SetCursor(NULL); return TRUE; } break;                 /* the mouse pointer is never shown over the game */
    case WM_SIZE: cw_ = LOWORD(l); ch_ = HIWORD(l); ev_push(VE_RESIZE, 0, 0, 0, 0); return 0;
    case WM_SETFOCUS: ev_push(VE_FOCUS, 0, 1, 0, 0); return 0;
    case WM_KILLFOCUS: ev_push(VE_FOCUS, 0, 0, 0, 0); return 0;
    case WM_KEYDOWN: case WM_SYSKEYDOWN: case WM_KEYUP: case WM_SYSKEYUP: {
        int down = (m == WM_KEYDOWN || m == WM_SYSKEYDOWN), rep = down && (l & (1 << 30)); int vk = (int)w;
        int ext = (l >> 24) & 1;                                           /* tell right Ctrl/Alt and numeric-keypad Enter apart */
        if (vk == VK_CONTROL) vk = ext ? VK_RCONTROL : VK_LCONTROL;
        if (vk == VK_RETURN && ext) vk = 0x10D;                              /* numeric-keypad Enter */
        int mods = (GetKeyState(VK_SHIFT) & 0x8000 ? 1 : 0) | (GetKeyState(VK_CONTROL) & 0x8000 ? 2 : 0) | (GetKeyState(VK_MENU) & 0x8000 ? 4 : 0);
        if (m == WM_SYSKEYDOWN && vk == VK_F4 && ((l >> 29) & 1)) { ev_push(VE_QUIT, 0, 0, 0, 0); return 0; }   /* Alt+F4 cierra */
        if (vk == VK_F10 || vk == VK_MENU) { ev_push(VE_KEY, vk, down, mods, rep); return 0; }
        ev_push(VE_KEY, vk, down, mods, rep); return 0;
    }
    case WM_SYSCOMMAND: if ((w & 0xFFF0) == SC_KEYMENU) return 0; break;
    }
    return DefWindowProcA(h, m, w, l);
}
int video_poll(VEvent *e) {
    MSG msg; while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) { TranslateMessage(&msg); DispatchMessageA(&msg); }
    if (!evq_n) return 0; *e = evq_[evq_h]; evq_h = (evq_h + 1) % EVQ; evq_n--; return 1;
}

/* ------------------------------------------------------------------------------------------------ shaders */
static const char *VERT_SRC = "#version 330\nin vec2 in_pos; in vec2 in_uv; out vec2 uv;\nvoid main() { gl_Position = vec4(in_pos, 0.0, 1.0); uv = in_uv; }\n";
static const char *COMMON_SRC =
"#version 330\n"
"uniform sampler2D tex;\n"
"uniform vec2 srcSize;\n"
"uniform vec2 texSize;\n"
"uniform vec2 outSize;\n"
"uniform float frame;\n"
"uniform int smooth_mode;\n"
"uniform float p_scan;\n"
"uniform float p_mask;\n"
"uniform float p_curv;\n"
"uniform float p_bright;\n"
"uniform float p_vig;\n"
"uniform float p_bloom;\n"
"in vec2 uv; out vec4 fragColor;\n"
"\n"
"vec3 fetch(vec2 c) {\n"
"    if (smooth_mode == 0) return texture(tex, (floor(c * texSize) + 0.5) / texSize).rgb;\n"
"    if (smooth_mode == 2) {\n"
"        vec2 p = c * srcSize; vec2 f = fract(p); vec2 i = floor(p);\n"
"        vec2 s = clamp((f - 0.5) * outSize / srcSize + 0.5, 0.0, 1.0);\n"
"        return texture(tex, (i + s) / srcSize).rgb;\n"
"    }\n"
"    return texture(tex, c).rgb;\n"
"}\n"
"vec2 curve(vec2 c, float k) {\n"
"    c = c * 2.0 - 1.0; vec2 o = abs(c.yx) * k * 0.25; c = c + c * o * o * 2.0; return c * 0.5 + 0.5;\n"
"}\n";
static const char *FRAG_NONE = "void main() { fragColor = vec4(fetch(uv) * p_bright, 1.0); }\n";
static const char *FRAG_SCAN =
"void main() {\n"
"    vec3 c = fetch(uv);\n"
"    float y = uv.y * srcSize.y; float d = fract(y) - 0.5;\n"
"    float w = 1.0 - p_scan * smoothstep(0.0, 0.5, abs(d)) * (0.35 + 0.65 * (1.0 - dot(c, vec3(0.299, 0.587, 0.114))));\n"
"    fragColor = vec4(c * w * p_bright, 1.0);\n"
"}\n";
static const char *FRAG_CRT =
"void main() {\n"
"    vec2 q = curve(uv, p_curv * 2.0);\n"
"    if (q.x < 0.0 || q.x > 1.0 || q.y < 0.0 || q.y > 1.0) { fragColor = vec4(0.0, 0.0, 0.0, 1.0); return; }\n"
"    vec3 c = fetch(q);\n"
"    if (p_bloom > 0.0) {\n"
"        vec2 o = 1.5 / srcSize;\n"
"        vec3 b = (fetch(q + vec2(o.x, 0.0)) + fetch(q - vec2(o.x, 0.0)) + fetch(q + vec2(0.0, o.y)) + fetch(q - vec2(0.0, o.y))) * 0.25;\n"
"        c = mix(c, max(c, b), p_bloom * 0.6) + b * p_bloom * 0.15;\n"
"    }\n"
"    float y = q.y * srcSize.y; float d = fract(y) - 0.5;\n"
"    float lum = dot(c, vec3(0.299, 0.587, 0.114));\n"
"    float beam = mix(0.55, 1.0, lum);\n"
"    float sl = 1.0 - p_scan * pow(clamp(abs(d) * 2.0, 0.0, 1.0), 2.0) * (1.6 - beam);\n"
"    float px = floor(gl_FragCoord.x);\n"
"    vec3 m = vec3(1.0);\n"
"    float ph = mod(px, 3.0);\n"
"    vec3 mk = vec3(ph < 1.0 ? 1.0 : 0.35, (ph >= 1.0 && ph < 2.0) ? 1.0 : 0.35, ph >= 2.0 ? 1.0 : 0.35);\n"
"    m = mix(vec3(1.0), mk * 1.35, p_mask);\n"
"    c = c * sl * m * p_bright;\n"
"    vec2 v = q * 2.0 - 1.0; c *= 1.0 - p_vig * dot(v, v) * 0.35;\n"
"    fragColor = vec4(c, 1.0);\n"
"}\n";
static const char *OV_FRAG = "#version 330\nuniform sampler2D tex; in vec2 uv; out vec4 fragColor;\nvoid main() { fragColor = texture(tex, uv); }\n";

static GLuint compile(GLenum type, const char *src) {
    GLuint s = p_glCreateShader(type); p_glShaderSource(s, 1, &src, NULL); p_glCompileShader(s);
    GLint ok = 0; p_glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) { GLchar_ log[400]; GLsizei n = 0; p_glGetShaderInfoLog(s, sizeof log - 1, &n, log); log[n] = 0; snprintf(err_, sizeof err_, "shader: %s", log); has_err = 1; p_glDeleteShader(s); return 0; }
    return s;
}
static GLuint make_program(const char *vs, const char *fs) {
    GLuint v = compile(GL_VERTEX_SHADER, vs), f = v ? compile(GL_FRAGMENT_SHADER, fs) : 0; if (!v || !f) return 0;
    GLuint p = p_glCreateProgram(); p_glAttachShader(p, v); p_glAttachShader(p, f); p_glLinkProgram(p);
    GLint ok = 0; p_glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) { GLchar_ log[400]; GLsizei n = 0; p_glGetProgramInfoLog(p, sizeof log - 1, &n, log); log[n] = 0; snprintf(err_, sizeof err_, "shader: %s", log); has_err = 1; p_glDeleteProgram(p); return 0; }
    p_glDeleteShader(v); p_glDeleteShader(f); return p;
}
static GLuint prog_builtin[3], prog_ov, prog_ext; static char prog_ext_name[64]; static float ext_par[64]; static char ext_par_name[64][48]; static int n_ext_par = 0;

static GLuint builtin(int which) {
    if (prog_builtin[which]) return prog_builtin[which];
    const char *body = which == 0 ? FRAG_NONE : which == 1 ? FRAG_SCAN : FRAG_CRT;
    size_t n = strlen(COMMON_SRC) + strlen(body) + 1; char *fs = (char *)malloc(n); snprintf(fs, n, "%s%s", COMMON_SRC, body);
    prog_builtin[which] = make_program(VERT_SRC, fs); free(fs); return prog_builtin[which];
}

static GLuint load_external(const char *path) {
    size_t n; uint8_t *d = read_file(path, &n); if (!d) return 0;
    char *src = (char *)malloc(n + 1); size_t k = 0; for (size_t i = 0; i < n; i++) if (d[i] != '\r') src[k++] = (char)d[i]; src[k] = 0; free(d);
    char *body = src; if (!strncmp(body, "#version", 8)) { while (*body && *body != '\n') body++; if (*body) body++; }
    n_ext_par = 0;
    for (char *p = body; (p = strstr(p, "#pragma parameter")) != NULL; ) {                    /* parameters with their default value */
        char name[48]; float def; if (sscanf(p, "#pragma parameter %47s \"%*[^\"]\" %f", name, &def) == 2 && n_ext_par < 64) { snprintf(ext_par_name[n_ext_par], 48, "%s", name); ext_par[n_ext_par++] = def; }
        p += 10;
    }
    size_t m = strlen(body) + 200; char *vs = (char *)malloc(m), *fs = (char *)malloc(m);
    snprintf(vs, m, "#version 330 compatibility\n#define VERTEX\n#define PARAMETER_UNIFORM\n%s", body); snprintf(fs, m, "#version 330 compatibility\n#define FRAGMENT\n#define PARAMETER_UNIFORM\n%s", body);
    GLuint p = make_program(vs, fs); free(vs); free(fs); free(src); return p;
}

static void scan_external(void) {
    n_ext = 0; if (!shaders_dir_[0]) return;
    char pat[600]; snprintf(pat, sizeof pat, "%s\\*.glsl", shaders_dir_); WIN32_FIND_DATAA fd; HANDLE h = FindFirstFileA(pat, &fd); if (h == INVALID_HANDLE_VALUE) return;
    do { if (n_ext < 64) { snprintf(ext_paths[n_ext], sizeof ext_paths[0], "%s/%s", shaders_dir_, fd.cFileName); snprintf(ext_names[n_ext], sizeof ext_names[0], "%s", fd.cFileName); char *dot = strrchr(ext_names[n_ext], '.'); if (dot) *dot = 0; n_ext++; } } while (FindNextFileA(h, &fd));
    FindClose(h);
}
int video_external_count(void) { return n_ext; }
const char *video_external_name(int i) { return i >= 0 && i < n_ext ? ext_names[i] : ""; }

void video_set_preset(const char *name) {
    static const struct { const char *n; int sh, flt; VidParams p; } P[4] = {
        { "None", 0, 0, { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f } }, { "Scanlines", 1, 1, { 0.45f, 0.0f, 0.0f, 1.08f, 0.0f, 0.0f } },
        { "Soft CRT", 2, 1, { 0.35f, 0.25f, 0.10f, 1.10f, 0.35f, 0.15f } }, { "Full CRT", 2, 1, { 0.65f, 0.55f, 0.22f, 1.25f, 0.60f, 0.50f } } };
    for (int i = 0; i < 4; i++) if (!strcmp(P[i].n, name)) { snprintf(vcfg.preset, sizeof vcfg.preset, "%s", name); vcfg.filter = P[i].flt; vcfg.p = P[i].p; vcfg.external[0] = 0; return; }
}
void video_use_external(const char *name) {
    snprintf(vcfg.external, sizeof vcfg.external, "%s", name); if (prog_ext) { p_glDeleteProgram(prog_ext); prog_ext = 0; } prog_ext_name[0] = 0;
}
static int preset_shader(void) { if (!strcmp(vcfg.preset, "None")) return 0; if (!strcmp(vcfg.preset, "Scanlines")) return 1; return 2; }

/* ------------------------------------------------------------------------------------------------ window and context */
int video_refresh_hz(void) { DEVMODEA dm; memset(&dm, 0, sizeof dm); dm.dmSize = sizeof dm; if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dm) && dm.dmDisplayFrequency > 1) return (int)dm.dmDisplayFrequency; return 60; }
void *video_hwnd(void) { return hwnd; }
void video_size(int *w, int *h) { *w = cw_; *h = ch_; }
const char *video_take_error(void) { if (!has_err) return NULL; has_err = 0; return err_; }
void video_title(const char *t) { SetWindowTextA(hwnd, t); }

static int soft_mode = 0;
static int open_gl(int w, int h, int sw, int sh, const char *shaders_dir) {
    { /* Without this Windows treats the program as "unscaled" and on a 4K screen with scaling it stretches the window by hand: fullscreen ran at 12 fps */
      typedef BOOL (WINAPI *PFN_dpictx)(HANDLE); HMODULE u = GetModuleHandleA("user32.dll"); PFN_dpictx f = u ? (PFN_dpictx)GetProcAddress(u, "SetProcessDpiAwarenessContext") : NULL;
      if (!f || !f((HANDLE)-4 /* PER_MONITOR_AWARE_V2 */)) { BOOL (WINAPI *g)(void) = u ? (BOOL (WINAPI *)(void))GetProcAddress(u, "SetProcessDPIAware") : NULL; if (g) g(); } }
    src_w_ = sw; src_h_ = sh; snprintf(shaders_dir_, sizeof shaders_dir_, "%s", shaders_dir ? shaders_dir : "");
    for (char *c = shaders_dir_; *c; c++) if (*c == '/') *c = 0x5C;
    HINSTANCE hi = GetModuleHandleA(NULL); WNDCLASSA wc; memset(&wc, 0, sizeof wc); wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW; wc.lpfnWndProc = wndproc; wc.hInstance = hi; wc.hCursor = NULL; wc.lpszClassName = "DowPortWnd";
    wc.hIcon = LoadIconA(hi, MAKEINTRESOURCEA(1)); RegisterClassA(&wc);
    RECT r = { 0, 0, w, h }; AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
    hwnd = CreateWindowExA(0, "DowPortWnd", "Dogs of War - port 60 fps", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, NULL, NULL, hi, NULL);
    if (!hwnd) return 0;
    hdc = GetDC(hwnd);
    PIXELFORMATDESCRIPTOR pfd; memset(&pfd, 0, sizeof pfd); pfd.nSize = sizeof pfd; pfd.nVersion = 1; pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; pfd.iPixelType = PFD_TYPE_RGBA; pfd.cColorBits = 32; pfd.iLayerType = PFD_MAIN_PLANE;
    int pf = ChoosePixelFormat(hdc, &pfd); if (!pf || !SetPixelFormat(hdc, pf, &pfd)) return 0;
    hrc = wglCreateContext(hdc); if (!hrc || !wglMakeCurrent(hdc, hrc)) return 0;
    LOADGL(glCreateShader); LOADGL(glShaderSource); LOADGL(glCompileShader); LOADGL(glGetShaderiv); LOADGL(glGetShaderInfoLog); LOADGL(glCreateProgram); LOADGL(glAttachShader); LOADGL(glLinkProgram);
    LOADGL(glGetProgramiv); LOADGL(glGetProgramInfoLog); LOADGL(glUseProgram); LOADGL(glGetUniformLocation); LOADGL(glGetAttribLocation); LOADGL(glUniform1i); LOADGL(glUniform1f); LOADGL(glUniform2f);
    LOADGL(glUniformMatrix4fv); LOADGL(glGenBuffers); LOADGL(glBindBuffer); LOADGL(glBufferData); LOADGL(glVertexAttribPointer); LOADGL(glEnableVertexAttribArray); LOADGL(glDisableVertexAttribArray);
    LOADGL(glActiveTexture); LOADGL(glDeleteProgram); LOADGL(glDeleteShader); LOADGL(glGenVertexArrays); LOADGL(glBindVertexArray);
    p_swapint = (PFN_swapint)wglGetProcAddress("wglSwapIntervalEXT");
    /* With swap interval 1 the driver waits for the VBL SPINNING (one core at 100 %). Interval 0 is preferred, waiting for the DWM composition (DwmFlush, blocks without using CPU);
       DOW_VSYNC=1 goes back to the classic vsync. */
    { HMODULE dwm = LoadLibraryA("dwmapi.dll"); if (dwm) p_dwmflush = (long (WINAPI *)(void))GetProcAddress(dwm, "DwmFlush"); }
    if (!p_dwmflush) force_vsync = 1; if (getenv("DOW_VSYNC")) force_vsync = 1;
    apply_sync();
    if (!p_glCreateShader || !p_glGenVertexArrays) return 0;
    glGenTextures(1, &tex_main); glBindTexture(GL_TEXTURE_2D, tex_main); glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, src_w_, src_h_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); tex_w_ = src_w_; tex_h_ = src_h_;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenTextures(1, &tex_ov); glBindTexture(GL_TEXTURE_2D, tex_ov); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    static const float Q[16] = { -1, -1, 0, 1, 1, -1, 1, 1, -1, 1, 0, 0, 1, 1, 1, 0 };
    p_glGenVertexArrays(1, &vao); p_glBindVertexArray(vao); p_glGenBuffers(1, &vbo); p_glBindBuffer(GL_ARRAY_BUFFER, vbo); p_glBufferData(GL_ARRAY_BUFFER, sizeof Q, Q, GL_STATIC_DRAW);
    if (!builtin(0)) return 0;                                              /* the program's shaders do not compile: the no-shader mode is used */
    scan_external(); ShowWindow(hwnd, SW_SHOW); SetForegroundWindow(hwnd);
    RECT cr; GetClientRect(hwnd, &cr); cw_ = cr.right; ch_ = cr.bottom;
    return 1;
}


/* ------------------------------------------------------------------------------------------------ software mode (GDI, no OpenGL or shaders)
 * If there is no OpenGL 3.3 (missing driver, remote desktop, virtual machine...) or the shaders do not compile, the image is scaled with GDI: same aspect, filter and menu, without CRT effects. */
static uint8_t *soft_src = NULL; static uint8_t *soft_ovl = NULL; static int soft_ow = 0, soft_oh = 0;
static HBITMAP soft_bm = NULL; static HDC soft_dc = NULL; static void *soft_bits = NULL; static int soft_bw = 0, soft_bh = 0;

static int open_soft(int w, int h, int sw, int sh) {
    soft_mode = 1; src_w_ = sw; src_h_ = sh; soft_src = (uint8_t *)calloc((size_t)sw * sh, 4);
    HINSTANCE hi = GetModuleHandleA(NULL); WNDCLASSA wc; memset(&wc, 0, sizeof wc); wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW; wc.lpfnWndProc = wndproc; wc.hInstance = hi; wc.hCursor = NULL; wc.lpszClassName = "DowPortWnd";
    wc.hIcon = LoadIconA(hi, MAKEINTRESOURCEA(1)); RegisterClassA(&wc);
    RECT r = { 0, 0, w, h }; AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
    hwnd = CreateWindowExA(0, "DowPortWnd", "Dogs of War - port 60 fps", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, NULL, NULL, hi, NULL);
    if (!hwnd) return 0;
    hdc = GetDC(hwnd); ShowWindow(hwnd, SW_SHOW); SetForegroundWindow(hwnd);
    RECT cr; GetClientRect(hwnd, &cr); cw_ = cr.right; ch_ = cr.bottom; host_log("OpenGL 3.3 not available: software mode (no shaders)");
    return 1;
}
static void soft_draw(void) {
    int ww = cw_, wh = ch_; if (ww < 1 || wh < 1) return;
    if (!(ov_on && soft_ovl) && !shot_path[0]) {                              /* no menu: straight to the window (faster); nearest-neighbor adjustment if the magnification is an integer */
        int x, y, w, h; video_rect(&x, &y, &w, &h);
        if (x > 0 || y > 0 || w < ww || h < wh) { RECT r = { 0, 0, ww, wh }; FillRect(hdc, &r, (HBRUSH)GetStockObject(BLACK_BRUSH)); }
        BITMAPINFO sb; memset(&sb, 0, sizeof sb); sb.bmiHeader.biSize = sizeof sb.bmiHeader; sb.bmiHeader.biWidth = src_w_; sb.bmiHeader.biHeight = -src_h_; sb.bmiHeader.biPlanes = 1; sb.bmiHeader.biBitCount = 32; sb.bmiHeader.biCompression = BI_RGB;
        SetStretchBltMode(hdc, (vcfg.filter == 0 || (w % src_w_ == 0 && h % src_h_ == 0)) ? COLORONCOLOR : HALFTONE); SetBrushOrgEx(hdc, 0, 0, NULL);
        StretchDIBits(hdc, x, y, w, h, 0, 0, src_w_, src_h_, soft_src, &sb, DIB_RGB_COLORS, SRCCOPY); return;
    }
    if (!soft_bm || soft_bw != ww || soft_bh != wh) {
        if (soft_bm) DeleteObject(soft_bm); if (soft_dc) DeleteDC(soft_dc);
        BITMAPINFO bi; memset(&bi, 0, sizeof bi); bi.bmiHeader.biSize = sizeof bi.bmiHeader; bi.bmiHeader.biWidth = ww; bi.bmiHeader.biHeight = -wh; bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32; bi.bmiHeader.biCompression = BI_RGB;
        soft_dc = CreateCompatibleDC(hdc); soft_bm = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &soft_bits, NULL, 0); SelectObject(soft_dc, soft_bm); soft_bw = ww; soft_bh = wh;
    }
    int x, y, w, h; video_rect(&x, &y, &w, &h);
    memset(soft_bits, 0, (size_t)ww * wh * 4);
    BITMAPINFO sb; memset(&sb, 0, sizeof sb); sb.bmiHeader.biSize = sizeof sb.bmiHeader; sb.bmiHeader.biWidth = src_w_; sb.bmiHeader.biHeight = -src_h_; sb.bmiHeader.biPlanes = 1; sb.bmiHeader.biBitCount = 32; sb.bmiHeader.biCompression = BI_RGB;
    SetStretchBltMode(soft_dc, vcfg.filter == 0 ? COLORONCOLOR : HALFTONE); SetBrushOrgEx(soft_dc, 0, 0, NULL);
    StretchDIBits(soft_dc, x, y, w, h, 0, 0, src_w_, src_h_, soft_src, &sb, DIB_RGB_COLORS, SRCCOPY);
    if (ov_on && soft_ovl && soft_ow == w && soft_oh == h) {                 /* menu: alpha blending over the image */
        uint8_t *dst = (uint8_t *)soft_bits;
        for (int j = 0; j < h; j++) {
            uint8_t *d = dst + ((size_t)(y + j) * ww + x) * 4; const uint8_t *o = soft_ovl + (size_t)j * w * 4;
            for (int i = 0; i < w; i++, d += 4, o += 4) { int a = o[3]; if (!a) continue; d[0] = (uint8_t)(d[0] + ((o[2] - d[0]) * a) / 255); d[1] = (uint8_t)(d[1] + ((o[1] - d[1]) * a) / 255); d[2] = (uint8_t)(d[2] + ((o[0] - d[0 + 2]) * a) / 255); }
        }
    }
    if (shot_path[0]) {
        uint8_t *px = (uint8_t *)malloc((size_t)ww * wh * 3); const uint8_t *b = (const uint8_t *)soft_bits;
        for (int i = 0; i < ww * wh; i++) { px[i * 3] = b[i * 4 + 2]; px[i * 3 + 1] = b[i * 4 + 1]; px[i * 3 + 2] = b[i * 4]; }
        png_write(shot_path, px, ww, wh, 3); free(px); shot_path[0] = 0;
    }
    BitBlt(hdc, 0, 0, ww, wh, soft_dc, 0, 0, SRCCOPY);
}

int video_is_software(void) { return soft_mode; }
void video_close(void);
int video_open(int w, int h, int sw, int sh, const char *shaders_dir) {
    if (!getenv("DOW_SOFTWARE") && open_gl(w, h, sw, sh, shaders_dir)) return 1;
    video_close(); return open_soft(w, h, sw, sh);
}

void video_close(void) { for (int i = 0; i < 3; i++) prog_builtin[i] = 0; prog_ov = 0; prog_ext = 0; prog_ext_name[0] = 0; if (soft_mode) { soft_mode = 0; free(soft_src); soft_src = NULL; free(soft_ovl); soft_ovl = NULL; if (soft_bm) { DeleteObject(soft_bm); soft_bm = NULL; } if (soft_dc) { DeleteDC(soft_dc); soft_dc = NULL; } }
    if (hrc) { wglMakeCurrent(NULL, NULL); wglDeleteContext(hrc); hrc = NULL; } if (hwnd && hdc) { ReleaseDC(hwnd, hdc); } if (hwnd) { DestroyWindow(hwnd); hwnd = NULL; } }

void video_upload(const uint8_t *rgb) {
    if (soft_mode) { size_t n = (size_t)src_w_ * src_h_; for (size_t i = 0; i < n; i++) { soft_src[i * 4] = rgb[i * 3 + 2]; soft_src[i * 4 + 1] = rgb[i * 3 + 1]; soft_src[i * 4 + 2] = rgb[i * 3]; } return; }
    glBindTexture(GL_TEXTURE_2D, tex_main);
    int S = src_w_ / 320; static uint8_t *small_ = NULL;
    if (vcfg.src_mode && S >= 1) {                                          /* TEST: the shaders always start from 320x200 */
        if (S > 1) {
            if (!small_) small_ = (uint8_t *)malloc(320 * 200 * 3);
            for (int y = 0; y < 200; y++) for (int x = 0; x < 320; x++) {
                uint8_t *d = small_ + ((size_t)y * 320 + x) * 3;
                if (vcfg.src_mode == 2) { const uint8_t *s = rgb + (((size_t)y * S + S / 2) * (size_t)src_w_ + (size_t)x * S + S / 2) * 3; d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; }
                else { unsigned a[3] = { 0, 0, 0 }; for (int j = 0; j < S; j++) for (int i = 0; i < S; i++) { const uint8_t *s = rgb + (((size_t)y * S + j) * (size_t)src_w_ + (size_t)x * S + i) * 3; a[0] += s[0]; a[1] += s[1]; a[2] += s[2]; }
                       unsigned n = (unsigned)(S * S); d[0] = (uint8_t)((a[0] + n / 2) / n); d[1] = (uint8_t)((a[1] + n / 2) / n); d[2] = (uint8_t)((a[2] + n / 2) / n); }
            }
        }
        if (tex_w_ != 320 || tex_h_ != 200) { glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 320, 200, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); tex_w_ = 320; tex_h_ = 200; }
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 320, 200, GL_RGB, GL_UNSIGNED_BYTE, S > 1 ? small_ : rgb); return;
    }
    if (tex_w_ != src_w_ || tex_h_ != src_h_) { glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, src_w_, src_h_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); tex_w_ = src_w_; tex_h_ = src_h_; }
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, src_w_, src_h_, GL_RGB, GL_UNSIGNED_BYTE, rgb); }

void video_overlay(const uint8_t *rgba, int w, int h) {
    if (!rgba) { ov_on = 0; return; }
    if (soft_mode) { free(soft_ovl); soft_ovl = (uint8_t *)malloc((size_t)w * h * 4); memcpy(soft_ovl, rgba, (size_t)w * h * 4); soft_ow = w; soft_oh = h; ov_on = 1; return; }
    glBindTexture(GL_TEXTURE_2D, tex_ov);
    if (w != ov_w || h != ov_h) { ov_w = w; ov_h = h; glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba); }
    else glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
    ov_on = 1;
}

void video_rect(int *x, int *y, int *w, int *h) {
    int ww = cw_, wh = ch_, a = vcfg.aspect;
    if (a == 2) { *x = 0; *y = 0; *w = ww; *h = wh; return; }
    if (a == 3) { int k = ww / 320 < wh / 200 ? ww / 320 : wh / 200; if (k < 1) k = 1; *w = 320 * k; *h = 200 * k; *x = (ww - *w) / 2; *y = (wh - *h) / 2; return; }
    double ratio = a == 0 ? 4.0 / 3.0 : 16.0 / 10.0; int w2 = ww, h2 = (int)lrint(w2 / ratio);
    if (h2 > wh) { h2 = wh; w2 = (int)lrint(h2 * ratio); }
    *w = w2; *h = h2; *x = (ww - w2) / 2; *y = (wh - h2) / 2;
}

static void u1f(GLuint p, const char *n, float v) { GLint l = p_glGetUniformLocation(p, n); if (l >= 0) p_glUniform1f(l, v); }
static void u2f(GLuint p, const char *n, float a, float b) { GLint l = p_glGetUniformLocation(p, n); if (l >= 0) p_glUniform2f(l, a, b); }
static void u1i(GLuint p, const char *n, int v) { GLint l = p_glGetUniformLocation(p, n); if (l >= 0) p_glUniform1i(l, v); }

static void draw_quad_builtin(GLuint p) {
    p_glBindVertexArray(vao); p_glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLint a0 = p_glGetAttribLocation(p, "in_pos"), a1 = p_glGetAttribLocation(p, "in_uv");
    if (a0 >= 0) { p_glEnableVertexAttribArray((GLuint)a0); p_glVertexAttribPointer((GLuint)a0, 2, GL_FLOAT, GL_FALSE, 16, (void *)0); }
    if (a1 >= 0) { p_glEnableVertexAttribArray((GLuint)a1); p_glVertexAttribPointer((GLuint)a1, 2, GL_FLOAT, GL_FALSE, 16, (void *)8); }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    if (a0 >= 0) p_glDisableVertexAttribArray((GLuint)a0); if (a1 >= 0) p_glDisableVertexAttribArray((GLuint)a1);
}
static void draw_quad_external(GLuint p) {
    static GLuint ebo = 0; static const float Q[16] = { -1, -1, 0, 1, 1, -1, 1, 1, -1, 1, 0, 0, 1, 1, 1, 0 };
    if (!ebo) { float d[32]; for (int i = 0; i < 4; i++) { d[i * 8 + 0] = Q[i * 4]; d[i * 8 + 1] = Q[i * 4 + 1]; d[i * 8 + 2] = 0; d[i * 8 + 3] = 1; d[i * 8 + 4] = Q[i * 4 + 2]; d[i * 8 + 5] = Q[i * 4 + 3]; d[i * 8 + 6] = 0; d[i * 8 + 7] = 1; }
        p_glGenBuffers(1, &ebo); p_glBindBuffer(GL_ARRAY_BUFFER, ebo); p_glBufferData(GL_ARRAY_BUFFER, sizeof d, d, GL_STATIC_DRAW); }
    p_glBindVertexArray(vao); p_glBindBuffer(GL_ARRAY_BUFFER, ebo);
    GLint a0 = p_glGetAttribLocation(p, "VertexCoord"), a1 = p_glGetAttribLocation(p, "TexCoord");
    if (a0 >= 0) { p_glEnableVertexAttribArray((GLuint)a0); p_glVertexAttribPointer((GLuint)a0, 4, GL_FLOAT, GL_FALSE, 32, (void *)0); }
    if (a1 >= 0) { p_glEnableVertexAttribArray((GLuint)a1); p_glVertexAttribPointer((GLuint)a1, 4, GL_FLOAT, GL_FALSE, 32, (void *)16); }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    if (a0 >= 0) p_glDisableVertexAttribArray((GLuint)a0); if (a1 >= 0) p_glDisableVertexAttribArray((GLuint)a1);
}

static void draw_overlay(void) {
    if (!ov_on) return;
    if (!prog_ov) prog_ov = make_program(VERT_SRC, OV_FRAG); if (!prog_ov) return;
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    p_glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex_ov); p_glUseProgram(prog_ov); u1i(prog_ov, "tex", 0); draw_quad_builtin(prog_ov);
    glDisable(GL_BLEND);
}

void video_draw(void) {
    if (soft_mode) { frame_n++; soft_draw(); return; }
    frame_n++; int ww = cw_, wh = ch_; if (ww < 1 || wh < 1) { SwapBuffers(hdc); return; }
    glViewport(0, 0, ww, wh); glClearColor(0, 0, 0, 1); glClear(GL_COLOR_BUFFER_BIT);
    int x, y, w, h; video_rect(&x, &y, &w, &h); glViewport(x, y, w, h);
    p_glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex_main);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, vcfg.filter == 0 ? GL_NEAREST : GL_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, vcfg.filter == 0 ? GL_NEAREST : GL_LINEAR);
    int done = 0;
    if (vcfg.external[0]) {
        for (int i = 0; i < n_ext && !done; i++) if (!strcmp(ext_names[i], vcfg.external)) {
            if (!prog_ext || strcmp(prog_ext_name, vcfg.external)) { if (prog_ext) p_glDeleteProgram(prog_ext); prog_ext = load_external(ext_paths[i]); snprintf(prog_ext_name, sizeof prog_ext_name, "%s", vcfg.external); }
            if (!prog_ext) { snprintf(err_, sizeof err_, "External shader %s failed", vcfg.external); has_err = 1; vcfg.external[0] = 0; break; }
            GLuint p = prog_ext; p_glUseProgram(p); static const float I[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
            GLint l = p_glGetUniformLocation(p, "MVPMatrix"); if (l >= 0) p_glUniformMatrix4fv(l, 1, GL_FALSE, I);
            u1i(p, "Texture", 0); u2f(p, "InputSize", 320.0f, 200.0f); u2f(p, "TextureSize", 320.0f, 200.0f); u2f(p, "OutputSize", (float)w, (float)h); u1i(p, "FrameCount", frame_n); u1i(p, "FrameDirection", 1);
            for (int k = 0; k < n_ext_par; k++) u1f(p, ext_par_name[k], ext_par[k]);
            draw_quad_external(p); done = 1;
        }
    }
    if (!done) {
        GLuint p = builtin(preset_shader()); if (p) {
            p_glUseProgram(p); u1i(p, "tex", 0); u2f(p, "srcSize", 320.0f, 200.0f); u2f(p, "texSize", (float)tex_w_, (float)tex_h_); u2f(p, "outSize", (float)w, (float)h);
            u1f(p, "frame", (float)frame_n); u1i(p, "smooth_mode", vcfg.filter);
            u1f(p, "p_scan", vcfg.p.p_scan); u1f(p, "p_mask", vcfg.p.p_mask); u1f(p, "p_curv", vcfg.p.p_curv); u1f(p, "p_bright", vcfg.p.p_bright); u1f(p, "p_vig", vcfg.p.p_vig); u1f(p, "p_bloom", vcfg.p.p_bloom);
            draw_quad_builtin(p);
        }
    }
    glViewport(x, y, w, h); draw_overlay();
    if (shot_path[0]) {
        uint8_t *px = (uint8_t *)malloc((size_t)ww * wh * 3), *fl = (uint8_t *)malloc((size_t)ww * wh * 3); glReadBuffer(GL_BACK); glPixelStorei(GL_PACK_ALIGNMENT, 1); glReadPixels(0, 0, ww, wh, GL_RGB, GL_UNSIGNED_BYTE, px);
        for (int yy = 0; yy < wh; yy++) memcpy(fl + (size_t)yy * ww * 3, px + (size_t)(wh - 1 - yy) * ww * 3, (size_t)ww * 3);
        png_write(shot_path, fl, ww, wh, 3); free(px); free(fl); shot_path[0] = 0;
    }
    SwapBuffers(hdc);
    if (use_dwm) p_dwmflush();
}
int video_screenshot(const char *path) { snprintf(shot_path, sizeof shot_path, "%s", path); return 1; }

void video_set_fullscreen(int on) {
    if (on == fullscreen) return;
    if (on) {
        GetWindowRect(hwnd, &win_rect); win_style = GetWindowLongA(hwnd, GWL_STYLE);
        MONITORINFO mi; mi.cbSize = sizeof mi; GetMonitorInfoA(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi);
        SetWindowLongA(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE); SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top - (getenv("DOW_FSFULL") ? 0 : -1), SWP_FRAMECHANGED | SWP_SHOWWINDOW);       /* 1 px TALLER (it sticks out at the bottom): it covers the whole monitor, so the taskbar hides, but since it is not exactly the same size Windows does not take it out of composition (independent flip, where DwmFlush does not synchronize) */
    } else {
        SetWindowLongA(hwnd, GWL_STYLE, win_style | WS_VISIBLE); SetWindowPos(hwnd, HWND_NOTOPMOST, win_rect.left, win_rect.top, win_rect.right - win_rect.left, win_rect.bottom - win_rect.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }
    fullscreen = on; RECT cr; GetClientRect(hwnd, &cr); cw_ = cr.right; ch_ = cr.bottom; apply_sync();
    if (getenv("DOW_TEST")) host_log("screen %s: client %dx%d, GL %s", on ? "fullscreen" : "windowed", cw_, ch_, (const char *)glGetString(GL_RENDERER));
}
int video_is_fullscreen(void) { return fullscreen; }
#endif
