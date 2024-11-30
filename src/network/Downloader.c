#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "Downloader.h"
#include "FileManager.h"
#include "Internet.h"

#define MAX_DATA_LENGTH 128
#define MAX_URL_LENGTH 200
#define MAX_PATH_LENGTH 64

char*
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
  sprintf(path, "./instances/%s/minecraft/bin/natives", version);
  sprintf(system_cmd, "mkdir -p %s", path);
  system(system_cmd);

  /* Define game JAR */
  sprintf(path, "./instances/%s/minecraft/bin/minecraft_%s.jar", version, type);

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
       dir[128] = {0}, file[128] = {0}, filepath[256] = {0},
       system_cmd[1024] = {0};

  switch (library)
  { case LWJGL:
    { sprintf(dir, "./libraries/org/lwjgl/lwjgl/%s", version);
      sprintf(file, "lwjgl-%s.zip", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://master.dl.sourceforge.net/project/java-game-lib/Official%%20Releases/LWJGL%%20%s/%s?viasf=1", version, file);
    } break;
    case AUTHLIB:
    { sprintf(dir, "./libraries/com/mojang/authlib/%s", version);
      sprintf(file, "authlib-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://papermc.io/repo/repository/maven-releases/com/mojang/authlib/%s/%s", version, file);
    } break;
    case APACHE_COMMONS_IO:
    { sprintf(dir, "./libraries/org/apache/commons/%s", version);
      sprintf(file, "commons-io-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://repo1.maven.org/maven2/commons-io/commons-io/%s/%s", version, file);
    } break;
    case APACHE_COMMONS_LANG3:
    { sprintf(dir, "./libraries/org/apache/commons/%s", version);
      sprintf(file, "commons-lang3-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://repo1.maven.org/maven2/org/apache/commons/commons-lang3/%s/%s", version, file);
    } break;
    case GSON:
    { sprintf(dir, "./libraries/com/google/gson/%s", version);
      sprintf(file, "gson-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://repo1.maven.org/maven2/com/google/code/gson/gson/%s/%s", version, file);
    } break;
    case GUAVA:
    { sprintf(dir, "./libraries/com/google/guava/%s", version);
      sprintf(file, "guava-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://repo1.maven.org/maven2/com/google/guava/guava/%s/%s", version, file);
    } break;
    case ICU4J:
    { sprintf(dir, "./libraries/com/ibm/icu/%s", version);
      sprintf(file, "icu4j-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://repo1.maven.org/maven2/com/ibm/icu/icu4j/%s/%s", version, file);
    } break;
    case JOPT_SIMPLE:
    { sprintf(dir, "./libraries/net/sf/jopt-simple/%s", version);
      sprintf(file, "jopt-simple-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://repo1.maven.org/maven2/net/sf/jopt-simple/jopt-simple/%s/%s", version, file);
    } break;
    case LOG4J_CORE:
    { sprintf(dir, "./libraries/org/apache/logging/%s", version);
      sprintf(file, "log4j-core-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://repo1.maven.org/maven2/org/apache/logging/log4j/log4j-core/%s/%s", version, file);
    } break;
    case LOG4J_API:
    { sprintf(dir, "./libraries/org/apache/logging/%s", version);
      sprintf(file, "log4j-api-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://repo1.maven.org/maven2/org/apache/logging/log4j/log4j-api/%s/%s", version, file);
    } break;
    case PAULSCODE_CODECJORBIS:
    { sprintf(dir, "./libraries/com/paulscode/%s", version);
      sprintf(file, "codecjorbis-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://nexus.velocitypowered.com/repository/maven-public/com/paulscode/codecjorbis/%s/%s", version, file);
    } break;
    case PAULSCODE_JAVA_SOUND:
    { sprintf(dir, "./libraries/com/paulscode/%s", version);
      sprintf(file, "libraryjavasound-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://nexus.velocitypowered.com/repository/maven-public/com/paulscode/libraryjavasound/%s/%s", version, file);
    } break;
    case PAULSCODE_LWJGL_OPENAL:
    { sprintf(dir, "./libraries/com/paulscode/%s", version);
      sprintf(file, "librarylwjglopenal-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://nifty-gui.sourceforge.net/nifty-maven-repo/com/paulscode/soundsystem/librarylwjglopenal/%s/%s", version, file);
    } break;
    case PAULSCODE_SOUND_SYSTEM:
    { sprintf(dir, "./libraries/com/paulscode/%s", version);
      sprintf(file, "soundsystem-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://nexus.velocitypowered.com/repository/maven-public/com/paulscode/soundsystem/%s/%s", version, file);
    } break;
    case NETTY_AIO:
    { sprintf(dir, "./libraries/io/netty/netty-all/%s", version);
      sprintf(file, "netty-all-%s.jar", version);
      sprintf(filepath, "%s/%s", dir, file);
      sprintf(url, "https://repo1.maven.org/maven2/io/netty/netty-all/%s/%s", version, file);
    } break;
    default: fprintf(stderr, "Not a valid library.\n"); return;
  }
  printf("Downloading library:\n\tNAME - %s\n\tPATH - %s\n", file, dir);
  /* TODO: Need support for Windows */
  sprintf(system_cmd, "mkdir -p %s", dir);
  system(system_cmd);

  CInternet.HandleData(filepath, url);

  /* Just for testing of course. */
  switch (library)
  { case LWJGL:
    { sprintf(system_cmd, "cd %s && unzip -qq %s", dir, file);
      system(system_cmd);
      sprintf(system_cmd, "cp %s/lwjgl-%s/jar/* ./instances/1.9/minecraft/bin", dir, version);
      system(system_cmd);
      sprintf(system_cmd, "cp %s/lwjgl-%s/native/linux/* ./instances/1.9/minecraft/bin/natives", dir, version);
      system(system_cmd);
    } break;
    default:
    { sprintf(system_cmd, "cp %s ./instances/1.9/minecraft/bin", filepath);
      system(system_cmd);
    } break;
  }
}

