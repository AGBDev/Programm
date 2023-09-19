#include "file_formats.h"
#include "defines.h"

void add_to_scp_file(scpfile_t *file, const char *name, const void *value)
{
}

void remove_from_scp_file_by_index(scpfile_t *file, size_t index)
{
}

void remove_from_scp_file_by_name(scpfile_t *file, const char *name)
{
}

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

    size_t f = fread(scp_file, sizeof(scpfile_t), 1, fp);
    if (f == 0)
    {
        return NULL;
    }

    fclose(fp);

    return scp_file;
}

size_t write_scp_file(const scpfile_t* scp_file, const char *filename)
{
    if (scp_file == NULL)
    {
        return 0;
    }

    FILE *fp;
    fopen_s(&fp, filename, "wb");
    if (fp == NULL)
    {
        return 0;
    }

    size_t f = fwrite(scp_file, sizeof(scpfile_t), 1, fp);
    if (f == 0) return 0;

    fclose(fp);

    return f;
}

scpfile_t *generate_scp_file(size_t number_of_entries)
{
    scpfile_t *file = malloc(sizeof(scpfile_t));

    file->number_of_entries = number_of_entries;
    file->entrie_names = malloc(sizeof(char) * number_of_entries);
    file->entrie_values = malloc(sizeof(void*) * number_of_entries);
    
    return file;
}