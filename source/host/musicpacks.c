/*
 * musicpacks.c - The music packs: subfolders of the music folder that contain audio files (the MP3 versions of the
 * game's songs). The audio files may also lie loose in the music folder itself; that counts as one pack whose name is
 * the empty string. The menu lists the packs found here; the emulation thread is told which folder to read.
 */
#include "app.h"

#define MAXPACK 32

static char packs[MAXPACK][80]; /* names found by the last scan_packs; "" = the files directly in music/ */
static int npacks = 0;

/* Does the folder contain an audio file? */
static int has_audio_files(const char *dir) {
#ifdef _WIN32
    char pat[760];
    snprintf(pat, sizeof pat, "%s/*", dir);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pat, &fd);
    if (h == INVALID_HANDLE_VALUE)
        return 0;
    int found = 0;
    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;
        const char *e = strrchr(fd.cFileName, '.');
        if (e && (!_stricmp(e, ".mp3") || !_stricmp(e, ".ogg") || !_stricmp(e, ".wav") || !_stricmp(e, ".wma") ||
                  !_stricmp(e, ".m4a"))) {
            found = 1;
            break;
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    return found;
#else
    (void)dir;
    return 0;
#endif
}

static int pack_cmp(const void *a, const void *b) {
    return _stricmp((const char *)a, (const char *)b);
}

/* Looks in the music folder: the loose files first (if any), then the subfolders in alphabetical order. */
void scan_packs(void) {
    npacks = 0;
#ifdef _WIN32
    if (has_audio_files(music_root))
        packs[npacks++][0] = 0;
    char pat[760];
    snprintf(pat, sizeof pat, "%s/*", music_root);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pat, &fd);
    int first = npacks;
    if (h != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || fd.cFileName[0] == '.' || npacks >= MAXPACK)
                continue;
            char d[800];
            snprintf(d, sizeof d, "%s/%s", music_root, fd.cFileName);
            if (has_audio_files(d))
                snprintf(packs[npacks++], 80, "%s", fd.cFileName);
        } while (FindNextFileA(h, &fd));
        FindClose(h);
    }
    qsort(packs[first], (size_t)(npacks - first), 80, pack_cmp);
#endif
}

int pack_count(void) {
    return npacks;
}

const char *pack_name(int i) {
    return i >= 0 && i < npacks ? packs[i] : "";
}

/* Index of the pack with that name (case-insensitive), or -1. */
int pack_find(const char *name) {
    for (int i = 0; i < npacks; i++)
        if (!_stricmp(packs[i], name))
            return i;
    return -1;
}

/* Full path of a pack's folder. */
void pack_dir(const char *name, char *out, size_t n) {
    if (name[0])
        snprintf(out, n, "%s/%s", music_root, name);
    else
        snprintf(out, n, "%s", music_root);
}

/* Leaves in music_pack a pack that exists: the requested one, or else the usual one ("arrange"), or else the first. */
void pack_resolve(const char *want) {
    int i = pack_find(want);
    if (i < 0)
        i = pack_find("arrange");
    if (i < 0)
        i = npacks > 0 ? 0 : -1;
    snprintf(music_pack, sizeof music_pack, "%s", i >= 0 ? packs[i] : want);
}
