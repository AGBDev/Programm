#include <stdlib.h>
#include <string.h>

#include "files.h"

scpfile_t *scpfile_generate()
{
    scpfile_t *file = malloc(sizeof(scpfile_t));
    if (file == NULL)
        return NULL;

    file->size = 0;
    file->key_names = NULL;  // Initialize to NULL
    file->key_values = NULL; // Initialize to NULL

    return file;
}

void scpfile_add(scpfile_t *scpfile, const char *name, size_t value)
{
    if (scpfile == NULL)
        return;

    // Dynamically allocate memory for new elements
    char **new_key_names = realloc(scpfile->key_names, sizeof(char *) * (scpfile->size + 1));
    size_t *new_key_values = realloc(scpfile->key_values, sizeof(size_t) * (scpfile->size + 1));

    if (new_key_names == NULL || new_key_values == NULL)
    {
        // Handle memory allocation failure here, e.g., by returning an error code.
        return;
    }

    // Assign the new key-value pair
    scpfile->key_names = new_key_names;
    scpfile->key_values = new_key_values;
    scpfile->key_names[scpfile->size] = strdup(name); // Make a copy of the name
    scpfile->key_values[scpfile->size] = value;
    scpfile->size++;
}

void scpfile_clear(scpfile_t *scpfile)
{
    if (scpfile == NULL)
    {
        return;
    }

    scpfile->size = 0;
}

void scpfile_free(scpfile_t *scpfile)
{
    free(scpfile->key_names);
    free(scpfile->key_values);
    free(scpfile);
}

size_t scpfile_save(scpfile_t *scpfile, const char *filename)
{
    if (scpfile == NULL)
        return 0;

    FILE *fp;
    fopen_s(&fp, filename, "wb");
    if (fp == NULL)
        return 0;

    fwrite(".SCP", sizeof(char) * 5, 1, fp);

    // Write the size field.
    fwrite(&scpfile->size, sizeof(size_t), 1, fp);

    // Write each string in key_names along with its length.
    for (size_t i = 0; i < scpfile->size; i++)
    {
        size_t name_len = strlen(scpfile->key_names[i]);
        fwrite(&name_len, sizeof(size_t), 1, fp);
        fwrite(scpfile->key_names[i], sizeof(char), name_len, fp);
    }

    // Write the key_values array.
    fwrite(scpfile->key_values, sizeof(size_t), scpfile->size, fp);

    fclose(fp);
    return 1; // Indicate success.
}

scpfile_t *scpfile_load(const char *filename)
{
    scpfile_t *scpfile = malloc(sizeof(scpfile_t));
    if (scpfile == NULL)
        return NULL;
    scpfile->size = 0;
    scpfile->key_names = NULL;  // Initialize to NULL initially
    scpfile->key_values = NULL; // Initialize to NULL initially

    FILE *fp;
    fopen_s(&fp, filename, "rb");
    if (fp == NULL)
    {
        free(scpfile);
        return NULL; // Return NULL to indicate a failure.
    }

    char str[] = ".SCP";
    char str_[5];
    fread(str_, sizeof(char[5]), 1, fp);
    if (strcmp(str, str_) != 0)
    {
        fclose(fp); // Close the file before returning NULL.
        free(scpfile);
        return NULL; // Return NULL to indicate a failure.
    }

    fread(&scpfile->size, sizeof(size_t), 1, fp);

    // Allocate memory for key_names and key_values based on the size.
    scpfile->key_names = malloc(sizeof(char *) * scpfile->size);
    scpfile->key_values = malloc(sizeof(size_t) * scpfile->size);

    if (scpfile->key_names == NULL || scpfile->key_values == NULL)
    {
        fclose(fp);
        free(scpfile->key_names);
        free(scpfile->key_values);
        free(scpfile);
        return NULL; // Return NULL to indicate a memory allocation failure.
    }

    // Read each string in key_names along with its length.
    for (size_t i = 0; i < scpfile->size; i++)
    {
        size_t name_len;
        fread(&name_len, sizeof(size_t), 1, fp);
        scpfile->key_names[i] = malloc(name_len + 1); // +1 for null-terminator
        if (scpfile->key_names[i] == NULL)
        {
            fclose(fp);
            // Handle memory allocation failure.
            for (size_t j = 0; j < i; j++)
            {
                free(scpfile->key_names[j]);
            }
            free(scpfile->key_names);
            free(scpfile->key_values);
            free(scpfile);
            return NULL;
        }
        fread(scpfile->key_names[i], sizeof(char), name_len, fp);
        scpfile->key_names[i][name_len] = '\0'; // Null-terminate the string.
    }

    // Read the key_values array.
    fread(scpfile->key_values, sizeof(size_t), scpfile->size, fp);
    fclose(fp);

    return scpfile;
}