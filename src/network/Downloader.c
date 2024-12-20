#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "data/settings.h"
#include "data/Application.h"
#include "data/Downloader.h"

#include "FileManager.h"
#include "Internet.h"

#define MAX_DATA_LENGTH 128
#define MAX_URL_LENGTH 200
#define MAX_PATH_LENGTH 64

static char*
extract_response_url(const char *data)
{
  int i = 0, b = 0;
  unsigned char start_copy = 0;
  char *buffer = malloc(sizeof(char)*strlen(data)+1);

  if (buffer == NULL)
  { fprintf(stderr, "Memory allocation failed.\n"); return NULL; }

  for (i=0; i<strlen(data); i++)
  { switch (data[i])
    { case '/': start_copy = 1; break;
      default: break;
    }

    if (start_copy)
    { buffer[b] = data[i];
      b++;
    }
  }

  buffer[b] = '\0';
  return buffer;
}

void
_D_FindGame(const char* version, const char* type, int force)
{
  if (version == NULL)
  { fprintf(stderr, "Not enough arguments! Expected a version argument\n"); return; }

  int file = 0;
  char data[MAX_DATA_LENGTH],
       path[MAX_PATH_LENGTH],
       url[MAX_URL_LENGTH];

  /* Retrieve data of url from BMCLAPI2. */
  sprintf(url, "https://bmclapi2.bangbang93.com/version/%s/%s", version, type);
  CInternet.HandleData("_file.temp", url);

  /* Read data. */
  file = CFileManager.OpenFile("_file.temp", "r");
  CFileManager.ReadLine(file, data, MAX_DATA_LENGTH);
  CFileManager.Flush(file);
  remove("_file.temp");

  /* Construct full URL */
  char* api = extract_response_url(data);
  sprintf(url, "https://launcher.mojang.com/%s", api);
  free(api);

  /* Construct local directory */
  char system_cmd[80] = {0};
  sprintf(path, "%s/bin/natives", mc.directory.game, version);
  sprintf(system_cmd, "mkdir -p %s", path);
  system(system_cmd);

  /* Define game JAR */
  sprintf(path, "%s/bin/minecraft_%s.jar", mc.directory.game, type);

  /* Check if local file already exists */
  if (force | (CFileManager.Validate(path) == 0))
  { printf("Saving into %s\n", path);
    CInternet.HandleData(path, url);
  }
  else
  { printf("Local file already exists; use -f flag to force download.\n"); }
}

void
_D_FindLibrary(Library library, const char* version)
{
  if (version == NULL)
  { fprintf(stderr, "Not enough arguments! Expected a version argument\n"); return; }

  char path[MAX_PATH_LENGTH] = {0},
       url[MAX_URL_LENGTH] = {0},
       dir[128] = {0},
       system_cmd[1024] = {0};

  struct localFILE
  { char name[128];
    char ext[16];
    char class[64];
    char fullname[160];
    char path[256];
  } file = {{0},{0},{0},{0},{0}};

  switch (library)
  { case LWJGL:
    { strcpy(file.name , "lwjgl");
      strcpy(file.ext  , "zip");
      strcpy(file.class, "org/lwjgl/lwjgl");
    } break;
    case AUTHLIB:
    { strcpy(file.name , "authlib");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "com/mojang/authlib");
    } break;
    case APACHE_COMMONS_IO:
    { strcpy(file.name , "commons-io");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "commons-io");
    } break;
    case APACHE_COMMONS_LANG3:
    { strcpy(file.name , "commons-lang3");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "org/apache/commons");
    } break;
    case GSON:
    { strcpy(file.name , "gson");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "com/google/code/gson");
    } break;
    case GUAVA:
    { strcpy(file.name , "guava");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "com/google/guava");
    } break;
    case ICU4J:
    { strcpy(file.name , "icu4j");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "com/ibm/icu");
    } break;
    case JOPT_SIMPLE:
    { strcpy(file.name , "jopt-simple");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "net/sf/jopt-simple");
    } break;
    case LOG4J_CORE:
    { strcpy(file.name , "log4j-core");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "org/apache/logging/log4j");
    } break;
    case LOG4J_API:
    { strcpy(file.name , "log4j-api");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "org/apache/logging/log4j");
    } break;
    case PAULSCODE_CODECJORBIS:
    { strcpy(file.name , "codecjorbis");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "com/paulscode/codecjorbis");
    } break;
    case PAULSCODE_CODECWAV:
    { strcpy(file.name , "codecwav");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "com/paulscode/codecwav");
    } break;
    case PAULSCODE_JAVA_SOUND:
    { strcpy(file.name , "libraryjavasound");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "com/paulscode/libraryjavasound");
    } break;
    case PAULSCODE_LWJGL_OPENAL:
    { strcpy(file.name , "librarylwjglopenal");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "com/paulscode/librarylwjglopenal");
    } break;
    case PAULSCODE_SOUND_SYSTEM:
    { strcpy(file.name , "soundsystem");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "com/paulscode/soundsystem");
    } break;
    case NETTY_AIO:
    { strcpy(file.name , "netty-all");
      strcpy(file.ext  , "jar");
      strcpy(file.class, "io/netty/netty-all");
    } break;
    default: fprintf(stderr, "Not a valid library.\n"); return;
  }
  sprintf(dir, "%s/libraries/%s/%s",application.path, file.class, version);
  sprintf(file.fullname, "%s-%s.%s", file.name, version, file.ext);
  sprintf(file.path, "%s/%s", dir, file.fullname);

  switch (library)
  { case LWJGL: sprintf(url, "https://master.dl.sourceforge.net/project/java-game-lib/Official%%20Releases/LWJGL%%20%s/%s?viasf=1", version, file.fullname); break;
    case APACHE_COMMONS_IO:
    case APACHE_COMMONS_LANG3:
    case GSON:
    case GUAVA:
    case ICU4J:
    case JOPT_SIMPLE:
    case LOG4J_CORE:
    case LOG4J_API: sprintf(url, "https://repo1.maven.org/maven2/%s/%s/%s/%s", file.class, file.name, version, file.fullname); break;
    default: sprintf(url, "https://repo.papermc.io/repository/maven-public/%s/%s/%s", file.class, version, file.fullname); break;
  }

  /* printf("Downloading library:\n\tNAME - %s\n\tPATH - %s\n", file.fullname, dir); */
  /* TODO: Need support for Windows */
  sprintf(system_cmd, "mkdir -p %s", dir);
  system(system_cmd);

  if (CFileManager.Validate(file.path))
  { fprintf(stderr, "File %s already exists in %s.\n", file.fullname, dir); }
  else
  { CInternet.HandleData(file.path, url); }

  /* Just for testing of course. */
  switch (library)
  { case LWJGL:
    { sprintf(system_cmd, "cd %s && unzip -n -qq %s", dir, file.fullname);
      system(system_cmd);
      sprintf(system_cmd, "cp %s/lwjgl-%s/jar/* %s/bin", dir, version, mc.directory.game);
      system(system_cmd);
      sprintf(system_cmd, "cp %s/lwjgl-%s/native/linux/* %s/bin/natives", dir, version, mc.directory.game);
      system(system_cmd);
    } break;
    default:
    { sprintf(system_cmd, "cp %s %s/bin", file.path, mc.directory.game);
      system(system_cmd);
    } break;
  }
}

