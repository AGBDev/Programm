#ifndef __FILES_H__
#define __FILES_H__

#include <stdio.h>

typedef struct _scp_file
{
    size_t size; //number of elements
    char **key_names;
    size_t *key_values;
} scpfile_t;

scpfile_t *scpfile_generate();
void scpfile_add(scpfile_t *scpfile, const char *name, size_t value);
void scpfile_clear(scpfile_t *scpfile);
void scpfile_free(scpfile_t *scpfile);
size_t scpfile_save(scpfile_t *scpfile, const char *filename);
scpfile_t *scpfile_load(const char *filename);

#endif