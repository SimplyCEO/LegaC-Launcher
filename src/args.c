#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Application.h"
#include "Downloader.h"

static struct GameType
{
  unsigned char client;
  unsigned char server;
} game;

unsigned char force_overwrite = 0,
              type_assign = 0, args = 0;

void
help()
{
  printf("%s - %s: A Minecraft launcher made in C from scratch.\n", application.name, application.version);
  printf("\n");
  printf("Usage:\n");
  printf("\t-t|--type:    The version type of the game (client/server).\n");
  printf("\t-f|--force:   Force overwriting existing files.\n");
  printf("\n");
  printf("Miscellaneous:\n");
  printf("\t-v|--verbose: Print every action. Normally used to debug.\n");
  printf("\t-h|--help:    Display help pages.\n");
}

unsigned char
parse_arguments(int argc, char* argv[])
{
  unsigned char i = 0, c = 0;

  char **args = malloc(sizeof(char*)*argc);
  for (i=0; i<argc; i++)
  { args[i] = malloc(sizeof(char)*32);
    /* Lowercase every entry. */
    for (c=0; c<(strlen(argv[i])+1); i++)
    { switch (argv[i][c])
      { case 'A' ... 'Z': args[i][c] = argv[i][c] + 32; break;
        default: args[i][c] = argv[i][c]; break;
      }
    }
  }

  for (i=1; i<argc; i++)
  { /* Help function. */
    if (!strncmp(args[i], "-h", 2) | !strncmp(args[i], "--help", 6))
    { help(); return 0; }
    /* Force overwrite checker. */
    else if (!strncmp(args[i], "-f", 2) | !strncmp(args[1], "--force", 7))
    { force_overwrite = 1; args++; }
    /* Game type reader. */
    else if (!strncmp(args[i], "-t", 2) | !strncmp(args[i], "--type", 6))
    { if (i+1<argc)
      { unsigned char type_args = 0;
        /* Single game type. */
        if (args[i+1])
        { switch (args[i+1][0])
          { case 'c': game.client = 1; type_args += 1; break;
            case 's': game.server = 1; type_args += 1; break;
            default: break;
          }
        }

        /* Dual game types */
        if (args[i+2])
        { switch (args[i+2][0])
          { case 'c': game.client = 1; type_args += 1; break;
            case 's': game.server = 1; type_args += 1; break;
            default: break;
          }
        }

        args += type_args+1;
        type_assign = 1;
      }
      else
      { fprintf(stderr, "Invalid syntax/Not enough arguments!\n"); return 1; }
    }
  }

  for (i=0; i<argc; i++)
  { free(args[i]); }
  free(args);

  return 0;
}

unsigned char
download_libraries(Library library, const char* version)
{
  CDownloader.FindLibrary(library, version);

  return 0;
}

unsigned char
download_game(const char* version, unsigned char from_gtk)
{
  /* Did not assign any type. */
  if (from_gtk == 1)
  { game.client = 1; game.server = 0; }
  else
  { if ((type_assign == 0) | ((game.client == 0) & (game.server == 0)))
    { fprintf(stderr, "Unknown given type.\n\n"); return 1; }
  }

  /* Download JAR */
  if (force_overwrite)
  { printf("Force override (-f) flag enabled.\n\n"); }

  printf("Downloading %s\n\n", version);

  if (game.client & game.server)
  { CDownloader.FindGame(version, "server", force_overwrite);
    CDownloader.FindGame(version, "client", force_overwrite);
  }
  else if (game.client)
  { CDownloader.FindGame(version, "client", force_overwrite); }
  else if (game.server)
  { CDownloader.FindGame(version, "server", force_overwrite); }
  else
  { fprintf(stderr, "Invalid argument for jar type!\n");
    return 1;
  }

  printf("All downloading complete.\n\n");

  return 0;
}

