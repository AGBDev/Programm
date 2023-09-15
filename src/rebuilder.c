#include <string.h>

#include "include/defines.h"
#include "include/rebuilder.h"
#include <sys/stat.h>
#include <unistd.h>

typedef rebuilder_file_t rebfile_t;

unsigned short rebuild_game(HWND hwnd)
{

    mkdir("halflife3");
    const char *content_dir = "halflife3/content";
    mkdir(content_dir);

    char *log = malloc(sizeof(char) * 500);
    if (log == NULL)
    {
        return 1;
    }
    strncpy_s(log, sizeof(char) * 500, "", 1);

    rebfile_t game_dat;
    game_dat.filename = "halflife3/content/game.common.gdat";

    if (build_file(game_dat, hwnd) == 0)
    {
        MessageBox(hwnd, "Error rebuilding!", MESSAGEBOX_TITLE, 0);
    }

    strncat_s(log, sizeof(char) * 500, game_dat.filename, 500 - strlen(log));

    rebfile_t game_anims;
    game_anims.filename = "halflife3/content/game.anims.gdat";

    if (build_file(game_anims, hwnd) == 0)
    {
        MessageBox(hwnd, "Error rebuilding!", MESSAGEBOX_TITLE, 0);
    }

    strncat_s(log, sizeof(char) * 500, game_anims.filename, 500 - strlen(log));

    rebfile_t game_levels;
    game_levels.filename = "halflife3/content/game.lvls.gdat";

    if (build_file(game_levels, hwnd) == 0)
    {
        MessageBox(hwnd, "Error rebuilding!", MESSAGEBOX_TITLE, 0);
    }

    strncat_s(log, sizeof(char) * 500, game_levels.filename, 500 - strlen(log));

    rebfile_t game_info;
    game_info.filename = "hl3.gaminfo";

    if (build_file(game_info, hwnd) == 0)
    {
        MessageBox(hwnd, "Error rebuilding!", MESSAGEBOX_TITLE, 0);
    }

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