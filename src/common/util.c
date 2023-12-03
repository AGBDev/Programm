#include "util.h"

size_t write_binary(const char *path, void *data, size_t size)
{
  FILE *file;
  fopen_s(&file, path, "wb");

  if (file == NULL) 
  {
    return 0;
  }

  size_t write = fwrite(data, size, 1, file);

  fclose(file);

  return write;
}
