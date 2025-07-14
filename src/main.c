#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#if defined(_WIN32)
#include "../assets/ffmpeg/ffmpeg_win.h"
#include "../assets/yt-dlp/yt-dlp_win.h"
#define FFMPEG_NAME "ffmpeg.exe"
#define YTDLP_NAME "yt-dlp.exe"
#define SHELL "cmd"
#define SHELL_ARG "/C"
#elif defined(__linux__)
#include "../assets/ffmpeg/ffmpeg_linux.h"
#include "../assets/yt-dlp/yt_dlp_linux.h"
#define FFMPEG_NAME "ffmpeg"
#define YTDLP_NAME "yt-dlp"
#define SHELL "sh"
#define SHELL_ARG "-c"
#elif defined(__APPLE__)
#include "../assets/ffmpeg/ffmpeg_macos.h"
#include "../assets/yt-dlp/yt_dlp_macos.h"
#define FFMPEG_NAME "ffmpeg"
#define YTDLP_NAME "yt-dlp"
#define SHELL "sh"
#define SHELL_ARG "-c"
#else
#error "Unsupported platform"
#endif

bool write_file(const char *path, const unsigned char *data, unsigned int size)
{
    FILE *f = fopen(path, "wb");
    if (!f)
    {
        return false;
    }
    fwrite(data, 1, size, f);
    fclose(f);
    return true;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: yt2mp3 <url> [-v]\n");
        return 1;
    }

    const char *url = argv[1];
    bool verbose = (argc >= 3 && strcmp(argv[2], "-v") == 0);

    if (!write_file(FFMPEG_NAME, ffmpeg_win, ffmpeg_win_len) ||
        !write_file(YTDLP_NAME, yt_dlp_win, yt_dlp_win_len))
    {
        fprintf(stderr, "Failed to extract binaries\n");
        return 1;
    }

    char command[4096];
    snprintf(command, sizeof(command),
             "./%s -x --audio-format mp3 --audio-quality 192K --ffmpeg-location ./%s --output %%(title)s.%%(ext)s \"%s\"",
             YTDLP_NAME, FFMPEG_NAME, url);

    if (verbose)
    {
        printf("Running: %s\n", command);
    }

    char *argv_exec[] = {SHELL, SHELL_ARG, command, NULL};
#ifdef _WIN32
    int code = system(command);
#else
    char *argv_exec[] = { SHELL, SHELL_ARG, command, NULL };
    int code = execvp(SHELL, argv_exec);
#endif

    remove(FFMPEG_NAME);
    remove(YTDLP_NAME);

    return code;
}
