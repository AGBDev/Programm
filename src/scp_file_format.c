#include "file_formats.h"
#include "defines.h"

//TODO: Make so it does work
scpfile_t *read_scp_file(const char *filename)
{
    scpfile_t *scp_file = malloc(sizeof(scp_file));
    if (scp_file == NULL)
    {
        return NULL;
    }

    FILE *fp;
    fopen_s(&fp, filename, "rb");
    if (fp == NULL)
    {
        return NULL;
    }

    // read file
    size_t *number_of_entries = malloc(sizeof(size_t));
    if (number_of_entries == NULL)
    {
        return NULL;
    }
    if (fread(number_of_entries, sizeof(size_t), 1, fp) == 0)
    {
        free(number_of_entries);
        return NULL;
    }

    scp_file->number_of_entries = *number_of_entries;

    fclose(fp);

    free(number_of_entries);
    return scp_file;
}

size_t write_scp_file(const scpfile_t* scp_file)
{
    if (scp_file == NULL)
    {
        return 0;
    }
}