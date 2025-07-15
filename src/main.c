#ifdef __linux__
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>

#ifdef _WIN32
#include <windows.h>
#include "../assets/ffmpeg/ffmpeg_win.h"
#include "../assets/yt-dlp/yt-dlp_win.h"
#define ffmpeg_data ffmpeg_win
#define ffmpeg_len ffmpeg_win_len
#define yt_dlp_data yt_dlp_win
#define yt_dlp_len yt_dlp_win_len
#define FFMPEG_NAME "ffmpeg.exe"
#define YTDLP_NAME "yt-dlp.exe"
#define SHELL "cmd"
#define SHELL_ARG "/C"
#define PATH_SEP "\\"
#else
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../assets/ffmpeg/ffmpeg_linux.h"
#include "../assets/yt-dlp/yt-dlp_linux.h"
#define ffmpeg_data ffmpeg_linux
#define ffmpeg_len ffmpeg_linux_len
#define yt_dlp_data yt_dlp_linux
#define yt_dlp_len yt_dlp_linux_len
#define FFMPEG_NAME "ffmpeg"
#define YTDLP_NAME "yt-dlp"
#define SHELL "sh"
#define SHELL_ARG "-c"
#define PATH_SEP "/"
#endif

#define TEMP_DIR_TEMPLATE "yt2mp3_tmpXXXXXX"

char ffmpeg_path[1024];
char yt_dlp_path[1024];
char temp_dir[1024];

void cleanup() {
    remove(ffmpeg_path);
    remove(yt_dlp_path);
#ifndef _WIN32
    rmdir(temp_dir);
#else
    RemoveDirectoryA(temp_dir);
#endif
}

void handle_signal(int sig) {
    cleanup();
    exit(1);
}

bool write_file(const char *path, const unsigned char *data, unsigned int size) {
    FILE *f = fopen(path, "wb");
    if (!f) {
        perror("fopen failed");
        return false;
    }
    if (fwrite(data, 1, size, f) != size) {
        perror("fwrite failed");
        fclose(f);
        return false;
    }
    fclose(f);
#ifndef _WIN32
    chmod(path, 0755);
#endif
    return true;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: yt2mp3 <url> [-v]\n");
        return 1;
    }

    const char *url = argv[1];
    bool verbose = (argc >= 3 && strcmp(argv[2], "-v") == 0);

    // Register cleanup
    atexit(cleanup);
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Create temp directory
#ifdef _WIN32
    char temp_path[MAX_PATH];
    GetTempPathA(sizeof(temp_path), temp_path);

    char temp_file[MAX_PATH];
    if (!GetTempFileNameA(temp_path, "yt2", 0, temp_file)) {
        fprintf(stderr, "GetTempFileNameA failed\n");
        return 1;
    }

    // Delete the temp file so we can create a directory in its place
    DeleteFileA(temp_file);
    if (!CreateDirectoryA(temp_file, NULL)) {
        fprintf(stderr, "Failed to create temp directory\n");
        return 1;
    }

    strncpy(temp_dir, temp_file, sizeof(temp_dir));
#else
    snprintf(temp_dir, sizeof(temp_dir), "/tmp/%s", TEMP_DIR_TEMPLATE);
    if (!mkdtemp(temp_dir)) {
        perror("mkdtemp failed");
        return 1;
    }
#endif

    snprintf(ffmpeg_path, sizeof(ffmpeg_path), "%s%s%s", temp_dir, PATH_SEP, FFMPEG_NAME);
    snprintf(yt_dlp_path, sizeof(yt_dlp_path), "%s%s%s", temp_dir, PATH_SEP, YTDLP_NAME);

    if (!write_file(ffmpeg_path, ffmpeg_data, ffmpeg_len) ||
        !write_file(yt_dlp_path, yt_dlp_data, yt_dlp_len)) {
        fprintf(stderr, "Failed to write binaries to temp dir\n");
        return 1;
    }

    char command[4096];
#ifdef _WIN32
    snprintf(command, sizeof(command),
        "%s %s \"%s -x --audio-format mp3 --audio-quality 192K --ffmpeg-location %s --output %%(title)s.%%(ext)s \"%s\"\"",
        SHELL, SHELL_ARG, yt_dlp_path, ffmpeg_path, url);

    if (verbose) {
        printf("Running: %s\n", command);
    }

    int code = system(command);
#else
    snprintf(command, sizeof(command),
             "/%s -x --audio-format mp3 --audio-quality 192K --ffmpeg-location /%s --output \"%%(title)s.%%(ext)s\" \"%s\"",
             yt_dlp_path, ffmpeg_path, url);

    if (verbose) {
        printf("Running: %s %s '%s'\n", SHELL, SHELL_ARG, command);
    }

    char *argv_exec[] = { SHELL, SHELL_ARG, command, NULL };
    int code = execvp(SHELL, argv_exec);
    if (code == -1) {
        perror("execvp failed");
        return 127;
    }
#endif

    return code;
}
