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

void add_to_scp_file(scpfile_t *file, const char *name, const void *value);
void remove_from_scp_file_by_index(scpfile_t *file, size_t index);
void remove_from_scp_file_by_name(scpfile_t *file, const char *name);
scpfile_t *read_scp_file(const char *filename);
size_t write_scp_file(const scpfile_t* scp_file, const char *filename);
scpfile_t *generate_scp_file(size_t number_of_entries);

#endif