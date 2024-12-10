#ifndef DOWNLOADER_HEADER
#define DOWNLOADER_HEADER

#include "data/Downloader.h"

void _D_FindGame(const char* version, const char* type, int force);
void _D_FindLibrary(Library library, const char* version);

static struct DownloaderClass
{
  void (*FindGame)(const char* version, const char* type, int force);
  void (*FindLibrary)(Library library, const char* version);
}
CDownloader =
{
  _D_FindGame, _D_FindLibrary
};

#endif

