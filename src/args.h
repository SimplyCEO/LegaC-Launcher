#ifndef MAIN_ARGS_HEADER
#define MAIN_ARGS_HEADER

#include "Downloader.h"

unsigned char parse_arguments(int argc, char* argv[]);
unsigned char download_libraries(Library library, const char* version);
unsigned char download_game(const char* version, unsigned char from_gtk);

#endif

