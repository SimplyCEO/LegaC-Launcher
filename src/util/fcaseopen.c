#include "fcaseopen.h"

#include <sys/stat.h>

#if !defined(_WIN32)
  #include <assert.h>
  #include <stdbool.h>
  #include <stdlib.h>
  #include <string.h>
  #include <strings.h>

  #include <dirent.h>
  #include <errno.h>
  #include <unistd.h>
  #include <ctype.h>

  #include <sys/types.h>

  char *strsep(char **stringp, const char *delim)
  {
    char *rv = *stringp;
    if (rv)
    {
      *stringp += strcspn(*stringp, delim);
      if (**stringp)
      { *(*stringp)++ = '\0'; }
      else
      { *stringp = 0; }
    }
    return rv;
  }

  char *trim(char *s) {
    char *ptr;
    if (!s)
    { return NULL; }  // handle NULL string
    if (!*s)
    { return s; }     // handle empty string
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    return s;
  }

  char*
  casepath(char const *path, bool checkPathFirst)
  {
    if (checkPathFirst && access(path, F_OK) != -1)
    {
      /* File path is correct. */
      return NULL;
    }

    size_t length = strlen(path);
    char *dir_path = (char*)malloc(sizeof(char)*(length+1));
    char *real_path = (char*)malloc(sizeof(char)*(length+3));
    strcpy(dir_path, path);
    size_t rp_length = 0;

    // my addon: linux doesn't handle filenames with spaces at the end nicely
    dir_path = trim(dir_path);
 
    DIR *directory = NULL;
    char *token = NULL;

    #if defined(__SWITCH__) || defined(PSP2)
      if((token = strstr(dir_path, ":/")) != NULL) // scheme used by some environments, eg. switch, vita
      {
        size_t deviceNameOffset = token - dir_path + 3;
        char* deviceNamePath = (char*)malloc(sizeof(char)*(deviceNameOffset+1));
        strlcpy(deviceNamePath, dir_path, deviceNameOffset);
        deviceNamePath[deviceNameOffset] = 0;
        directory = opendir(deviceNamePath);
        dir_path = token + 1;
        free(deviceNamePath);
      }
      else
    #endif
    {
      switch (dir_path[0])
      {
        case '/':
        case '\\':
        {
          directory = opendir("/");
          dir_path = dir_path + 1;
        } break;
        default:
        {
          directory = opendir(".");
          real_path[0] = '.';
          real_path[1] = 0;
          rp_length = 1;
        } break;
      }
    }
    
    bool cantProceed = false; // just convert slashes in what's left in string, don't correct case of letters(because we can't)
    bool mayBeTrailingSlash = false;
    while ((token = strsep(&dir_path, "/\\")))
    {
      // May be trailing slash(allow), slash at the start(avoid), or multiple slashes(avoid)
      switch (*token)
      {
        case '\0': mayBeTrailingSlash = true; continue;
        default: mayBeTrailingSlash = false; break;
      }
      
      real_path[rp_length] = '/';
      rp_length += 1;
      real_path[rp_length] = 0;

      switch (cantProceed)
      {
        case 0: break;
        default:
        {
          strcpy(real_path + rp_length, token);
          rp_length += strlen(token);
        } continue;
      }
      
      struct dirent* entry = readdir(directory);
      bool validation = false;
      while (entry && !validation)
      {
        switch (strcasecmp(token, entry->d_name))
        {
          case 0:
          {
            strcpy(real_path + rp_length, entry->d_name);
            int rp_len_assert = (int)strlen(entry->d_name);
            rp_length += rp_len_assert;
            assert(rp_len_assert == strlen(token) && "casepath: This is not good at all");

            closedir(directory);
            directory = opendir(real_path);

            // Either it wasn't a folder, or permission error, I/O error etc.
            if (!directory)
            { cantProceed = true; }

            validation = true;
          } break;
          default: entry = readdir(directory); break;
        }
      }
      
      if (!entry)
      {
        /* printf("casepath couldn't find dir/file \"%s\", full path was %s\n", token, path); */
        // No match, add original name and continue converting further slashes.
        strcpy(real_path + rp_length, token);
        rp_length += strlen(token);
        cantProceed = true;
      }
    }
    
    if (directory)
    { closedir(directory); }

    if (mayBeTrailingSlash)
    {
      real_path[rp_length] = '/';  rp_length += 1;
      real_path[rp_length] = '\0';
    }

    if (rp_length > length + 2)
    {
      printf("\n\ncasepath: Corrected path length is longer then original+2:\n\tOriginal: %s (%zu chars)\n\tCorrected: %s (%zu chars)\n\n", path, length, real_path, rp_length);
    }

    free(dir_path);
    return real_path;
  }

  int
  _caserename(const char *old_filename, const char *new_filename)
  {
      int result;
      char *real_old = casepath(old_filename, true);
      char *real_new = casepath(new_filename, true);

      // hack so we don't even try to rename it to new_filename if it already exists
      if (!real_new) {
        free(real_old);
        return -1;
      }

      if (!real_old)
        result = rename(old_filename, real_new);
      else
        result = rename(real_old, real_new);

      free(real_old);
      free(real_new);

      return result;
  }

  FILE*
  _fcaseopen(char const *path, char const *mode)
  {
    FILE *stream = fopen(path, mode);
    #if !defined(_WIN32)
      if (!stream)
      {
        char *real_path = casepath(path, true);
        if (!real_path)
        { stream = fopen(path, mode); }
        else
        {
          stream = fopen(real_path, mode);
          free(real_path);
        }
      }
    #endif
    return stream;
  }

  void
  _casechdir(char const *path)
  {
    #if !defined(_WIN32)
      char *real_path = casepath(path, true);
      if (real_path)
      {
        chdir(real_path);
        free(real_path);
      }
      else
      { errno = ENOENT; }
    #else
      chdir(path);
    #endif
  }
#endif

