#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "fcaseopen.h"

#define MAX_FM_FILES 10
static struct ArchivedFiles
{ char path[256];
  unsigned char txt;
  FILE* stream;
} file[MAX_FM_FILES];

static int
_fopen(const char* filepath, const char* mode)
{
  int fd = 0;
  char realmode[10], *p;

  for (fd=1; fd<MAX_FM_FILES; fd++)
  { if (file[fd].stream == NULL)
    { goto found; }
  }
  return 0;
found:
  file[fd].txt = strchr(mode, 'b') == NULL;
  p = realmode;
  while (*mode)
  { switch (*mode)
    { case 'b': case 't': mode++; break;
      default: *p++ = *mode++; break;
    }
  }
  *p++ = 'b';
  *p = '\0';

  #if !defined(_WIN32)
    char* real_path = casepath(filepath, true);
    //strncpy(file[fd].path, real_path, 256);
    free(real_path);
  #else
    strncpy(file[fd].path, filepath, 256);
  #endif
  //file[fd].stream = fopen(file[fd].path, realmode);
  file[fd].stream = fcaseopen(filepath, realmode);

  if (file[fd].stream == NULL)
  { return 0; }
  return fd;
}

static int
_fclose(int fd)
{
  int result = 0;
  assert(fd < MAX_FM_FILES);
  if (file[fd].stream)
  { result = fclose(file[fd].stream);
    file[fd].stream = NULL;
    return result;
  }
  return EOF;
}

static int
_fgetc(int fd)
{
  char c = 0;
  c = fgetc(file[fd].stream);
  if (file[fd].txt && c == 015)
  { /* Translate CRLF to LF. */
    c = fgetc(file[fd].stream);
    if (c == 012)
    { return c; }
    ungetc(c, file[fd].stream);
    return 015;
  }
  return c;
}

static int
_fputc(char c, int fd)
{
  /* Translate LF to CRLF. */
  if (file[fd].txt && c == 012)
  { fputc(015, file[fd].stream); }
  return fputc(c, file[fd].stream);
}

static char*
_fgets(char* buf, int len, int fd)
{
  char c, *p;

  p = buf;
  len--; /* NULL byte */
  while (len--)
  { c = _fgetc(fd);
    if (c == EOF)
    { if (p == buf)
      { return NULL; }
      break;
    }
    *p++ = c;
    if (c == '\n')
    { break; }
  }
  *p = '\0';
  return buf;
}

static int
_fread(void* buf, int elt, int n, int fd)
{
  if (file[fd].txt)
  { unsigned char *p;
    int i;
    char c;

    n *= elt;
    p = (unsigned char*)buf;
    for (i=0; i<n; i++)
    { c = _fgetc(fd);
      if (c == EOF)
      { break; }
      *p++ = (unsigned char)c;
    }
    return i / elt;
  }
  return fread(buf, elt, n, file[fd].stream);
}

static int
_fwrite(void* buf, int elt, int n, int fd)
{
  if (file[fd].txt)
  { unsigned char *p;
    int i;
    char c;

    n *= elt;
    p = (unsigned char*)buf;
    for (i=0; i<n; i++)
    { c = *p++;
      _fputc(c, fd);
      if (feof(file[fd].stream))
      { break; }
    }
    return i / elt;
  }
  return fwrite(buf, elt, n, file[fd].stream);
}

static int
_fseek(int fd, long offset, int whence)
{
  return fseek(file[fd].stream, offset, whence);
}

static int
_feof(int fd)
{
  return feof(file[fd].stream);
}

int
_FM_Validate(const char* filepath)
{
  FILE* fp = fopen(filepath, "r");
  if (fp != NULL)
  { fclose(fp); return 1; }
  return 0;
}

int
_FM_OpenFile(const char* filepath, const char* mode)
{
  return _fopen(filepath, mode);
}

int
_FM_Read(int fd, const char* buf, int len)
{
  return _fread((void*)buf, 1, len, fd);
}

int
_FM_Write(int fd, const char* buf, int len)
{
  return _fwrite((void*)buf, 1, len, fd);
}

unsigned char
_FM_Seek(int fd, int offset, int whence)
{
  return !!_fseek(fd, offset, whence);
}

unsigned char
_FM_ReadLine(int fd, char* buf, int len)
{
  return _fgets(buf, len, fd) != NULL;
}

int
_FM_CloseFile(int fd)
{
  return _fclose(fd);
}

int
_FM_GetError(int fd)
{
  return _feof(fd);
}

FILE*
_FM_Stream(int fd)
{
  return file[fd].stream;
}

void
_FM_Copy(int fd1, int fd2)
{
  int c = 0;
  while ((c = fgetc(file[fd1].stream)) != EOF)
  { fputc(c, file[fd2].stream); }
}

void
_FM_CopyFromPath(const char* filepath1, const char* filepath2)
{
  int fd1 = _FM_OpenFile(filepath1, "r");
  if (fd1 == 0)
  { fprintf(stderr, "Could not open '%s' path of file.", filepath1);
    _FM_CloseFile(fd1);
    return;
  }

  int fd2 = _FM_OpenFile(filepath2, "w");

  int c = 0;
  while ((c = fgetc(file[fd1].stream)) != EOF)
  { fputc(c, file[fd2].stream); }

  _FM_CloseFile(fd1);
  _FM_CloseFile(fd2);
}

void
_FM_Flush(int fd)
{
  _fclose(fd);
  remove(file[fd].path);
  file[fd].path[0] = '\0';
  file[fd].txt = 0;
}

char*
_FM_ReadFile(int fd)
{
  _FM_Seek(fd, 0, SEEK_END);

  long file_size = ftell(file[fd].stream);
  rewind(file[fd].stream);

  char* content = malloc(file_size+1);
  fread(content, file_size, 1, file[fd].stream);
  content[file_size] = '\0';

  return content;
}

char*
_FM_ReadFileFromPath(const char* filepath)
{
  int fd = _FM_OpenFile(filepath, "r");
  char* content = _FM_ReadFile(fd);
  _FM_CloseFile(fd);

  return content;
}

