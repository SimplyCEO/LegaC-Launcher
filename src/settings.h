#ifndef SETTINGS_CONFIGURATION_HEADER
#define SETTINGS_CONFIGURATION_HEADER

typedef struct
{ struct MCVer
  { unsigned char major;
    unsigned char minor;
    unsigned char patch;
  } version;
  char username[16];
  struct MCDir
  { char game[128];
    char asset[128];
  } directory;
} MinecraftSettings;
extern MinecraftSettings mc;

#endif

