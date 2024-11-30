#if !defined(FCASEOPEN_HEADER)
#define FCASEOPEN_HEADER

#include <stdio.h>
#include <stdbool.h>

#if defined(_WIN32)
  #define fcaseopen fopen
  #define caserename rename
#else
  char *casepath(char const *path, bool checkPathFirst);
  int   _caserename(const char *old_filename, const char *new_filename);
  FILE *_fcaseopen(char const *path, char const *mode);
  void  _casechdir(char const *path);
  #define fcaseopen _fcaseopen
  #define caserename _caserename
#endif

#endif

