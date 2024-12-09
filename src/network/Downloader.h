#ifndef DOWNLOADER_HEADER
#define DOWNLOADER_HEADER

typedef enum
{ LWJGL = 0,
  AUTHLIB,
  APACHE_COMMONS_IO,
  APACHE_COMMONS_LANG3,
  GSON,
  GUAVA,
  ICU4J,
  JOPT_SIMPLE,
  LOG4J_CORE,
  LOG4J_API,
  PAULSCODE_CODECJORBIS,
  PAULSCODE_CODECWAV,
  PAULSCODE_JAVA_SOUND,
  PAULSCODE_LWJGL_OPENAL,
  PAULSCODE_SOUND_SYSTEM,
  NETTY_AIO
} Library;

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

