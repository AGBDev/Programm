#include <stdlib.h>

#include "files.h"

scpfile_t *scpfile_generate(const char *name)
{
    scpfile_t *file = malloc(sizeof(scpfile_t));
    if (file == NULL) return NULL;

    file->name = (char*)name;
    file->size = 0;
    file->key_names = malloc(sizeof(char*));
    if (file->key_names == NULL)
    {
        free(file);
        return NULL;
    }
    file->key_values = malloc(sizeof(int));
    if (file->key_values == NULL)
    {
        free(file->key_names);
        free(file);
        return NULL;
    }

    return file;
}

void scpfile_clear(scpfile_t *scpfile)
{
    if (scpfile == NULL)
    {
        return;
    }
    scpfile->size = 0;
    scpfile->key_names = realloc(scpfile->key_names, sizeof(char*));
    scpfile->key_values = realloc(scpfile->key_values, sizeof(int));
}

size_t scpfile_save(scpfile_t *scpfile, const char *filename)
{
    if (scpfile == NULL) return 0;

    FILE *fp;
    fopen_s(&fp, filename, "wb");
    if (fp == NULL) return 0;
    fwrite(scpfile, sizeof scpfile, 1, fp);
    fclose(fp);
    return 0;
}

scpfile_t *scpfile_load(const char *filename)
{
    return NULL;
}
