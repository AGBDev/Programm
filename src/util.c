#include "include/util.h"

unsigned short file_exists(const char* filename)
{
    if (strlen(filename) == 0) return 0;

    FILE *fp;
    fopen_s(&fp, filename, "r");
    
    if (fp == NULL) return 0;
    fclose(fp);
    return 1;
}