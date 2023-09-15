#ifndef __REBUILDER_H__
#define __REBUILDER_H__

#include <windows.h>
#include <stdio.h>

typedef struct reb_file_data
{
    size_t size_of_data;
    void **data_array;
} rebuilder_file_data_t;

typedef struct reb_file 
{
    char *filename;
    
} rebuilder_file_t;

unsigned short rebuild_game(HWND hwnd);
unsigned int build_file(rebuilder_file_t file, HWND hwnd);

#endif // __REBUILDER_H__