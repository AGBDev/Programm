#ifndef __FILE_FORMATS_H__
#define __FILE_FORMATS_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct _scp_file
{
    size_t number_of_entries;
    char *entrie_names;
    void *entrie_values;
} scpfile_t;

scpfile_t *read_scp_file(const char *filename);
size_t write_scp_file(const scpfile_t* scp_file);

#endif