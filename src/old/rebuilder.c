#include <string.h>

#include "defines.h"
#include "rebuilder.h"
#include <sys/stat.h>
#include <unistd.h>

typedef rebuilder_file_t rebfile_t;

unsigned short rebuild_game(HWND hwnd)
{

    mkdir("data");
    const char *content_dir = "data/content";
    mkdir(content_dir);

    char *log = malloc(sizeof(char) * 500);
    if (log == NULL)
    {
        return 1;
    }
    strncpy_s(log, sizeof(char) * 500, "", 1);

    rebfile_t game_dat;
    game_dat.filename = "data/content/common.cres";

    if (build_file(game_dat, hwnd) == 0)
    {
        MessageBox(hwnd, "Error rebuilding!", MESSAGEBOX_TITLE, 0);
    }

    strncat_s(log, sizeof(char) * 500, game_dat.filename, 500 - strlen(log));

    rebfile_t game_anims;
    game_anims.filename = "data/content/anims.cres";

    if (build_file(game_anims, hwnd) == 0)
    {
        MessageBox(hwnd, "Error rebuilding!", MESSAGEBOX_TITLE, 0);
    }

    strncat_s(log, sizeof(char) * 500, game_anims.filename, 500 - strlen(log));

    rebfile_t game_levels;
    game_levels.filename = "data/content/windows.cres";

    if (build_file(game_levels, hwnd) == 0)
    {
        MessageBox(hwnd, "Error rebuilding!", MESSAGEBOX_TITLE, 0);
    }

    strncat_s(log, sizeof(char) * 500, game_levels.filename, 500 - strlen(log));

    // handled differently
    rebfile_t game_info;
    game_info.filename = "prog.inf";

    FILE *fp_;
    fopen_s(&fp_, game_info.filename, "w");
    if (fp_ == NULL)
    {
        MessageBox(hwnd, "Error rebuilding!", MESSAGEBOX_TITLE, 0);
    }
    fprintf_s(fp_, "version=%s", WINDOW_HEADER);
    fclose(fp_);

    strncat_s(log, sizeof(char) * 500, game_info.filename, 500 - strlen(log));

    mkdir("temp");
    FILE *fp;
    fopen_s(&fp, "temp/rebuildlog.txt", "w");
    if (fp == NULL) return 1;
    fprintf_s(fp, "%s", log);
    fclose(fp);

    MessageBox(hwnd, "Rebuilding done. \nLog can be found at 'temp/rebuildlog.txt'!", MESSAGEBOX_TITLE, 0);
    free(log);

    return 0;
}

unsigned int build_file(rebuilder_file_t file, HWND hwnd)
{
    if (strlen(file.filename) == 0 || strlen(file.filename) > 100)
    {
        return 0;
    }

    FILE *fp;
    if (fopen_s(&fp, file.filename, "wb") != 0)
    {
        MessageBox(hwnd, "Failed to open file!", MESSAGEBOX_TITLE, 0);
        return 0;
    }

    size_t size_of_written_data = fwrite(file.filename, sizeof(char) * strlen(file.filename), 999, fp);

    // if (size_of_written_data != (sizeof(char) * strlen(file.filename)))
    // { 
    //     MessageBox(hwnd, "Failed to write all data!", MESSAGEBOX_TITLE, 0);
    //     fclose(fp);
    //     return 0;
    // }

    fclose(fp);


    return size_of_written_data;
}