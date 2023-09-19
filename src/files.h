#ifndef __FILES_H__
#define __FILES_H__

#include <stdio.h>

typedef struct _scp_file
{
    char *name; //filename
    size_t size; //number of elements
    char **key_names; //key names (keynames[1] -> keyvalues[1])
    size_t *key_values; //key values
} scpfile_t;

scpfile_t *scpfile_generate(const char *name);
void scpfile_clear(scpfile_t *scpfile);
size_t scpfile_save(scpfile_t *scpfile, const char *filename);
scpfile_t *scpfile_load(const char *filename);

#endif