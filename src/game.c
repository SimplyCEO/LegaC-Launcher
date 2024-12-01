#include <string.h>
#include <stdlib.h>

#include "args.h"
#include "game.h"
#include "settings.h"
#include "Downloader.h"

void
play_game(const char* mc_version, const char* mc_username, const char* mc_gamedir, const char* mc_class)
{
  char java_buffer[2048] = {0};
  sprintf(java_buffer, "java -Xms128M -Xmx1024M");
  #if !defined(_WIN32)
    sprintf(java_buffer, "%s -Dorg.lwjgl.openal.libname=/lib/libopenal.so", java_buffer);
  #endif
  sprintf(java_buffer, "%s -Djava.library.path='%s/bin/natives' -cp '%s/bin/*' %s --username %s --version %s --accessToken 0 --userProperties {} --gameDir %s --assetsDir ./assets --width 800 --height 600", java_buffer, mc_gamedir, mc_gamedir, mc_class, mc_username, mc_version, mc_gamedir);
  /* printf("JAVA COMMAND: %s\n", java_buffer); */
  system(java_buffer);
}

void log_message(LogDisplay *logger, const gchar *message)
{ g_async_queue_push(logger->queue, g_strdup(message)); }
void
initialise_game(LogDisplay* log_display)
{
  /* Build game configuration */
  char mc_version[12] = {0};
  if (mc.version.patch > 0)
  { sprintf(mc_version, "%hhu.%hhu.%hhu", mc.version.major, mc.version.minor, mc.version.patch); }
  else
  { sprintf(mc_version, "%hhu.%hhu", mc.version.major, mc.version.minor); }

  char mc_username[16] = {0};
  if (mc.username[0] != '\0')
  { strncpy(mc_username, mc.username, 16); }
  else
  { strcpy(mc_username, "LegaC_user");
    strcpy(mc.username, mc_username);
  }

  char mc_gamedir[128] = {0};
  sprintf(mc_gamedir, "./instances/%s/minecraft", mc_version);
  strcpy(mc.directory.game, mc_gamedir);

  char mc_class[32] = {0};
  if ((mc.version.major == 1) & (mc.version.minor > 5))
  { strcpy(mc_class, "net.minecraft.client.main.Main"); }
  else
  { strcpy(mc_class, "net.minecraft.client.Minecraft"); }

  /* printf("Strings: %s %s %s %s\n", mc_version, mc_username, mc_gamedir, mc_class); */

  /* Download dependencies and run game */
  log_message(log_display, "Downloading Minecraft game...\n");
  download_game(mc_version, 1);
  log_message(log_display, "Downloading LWJGL library...\n");
  download_libraries((Library)LWJGL, "2.9.3");
  log_message(log_display, "Downloading AUTHLIB library...\n");
  download_libraries((Library)AUTHLIB, "1.5.25");
  log_message(log_display, "Downloading APACHE_COMMONS_IO library...\n");
  download_libraries((Library)APACHE_COMMONS_IO, "2.5");
  log_message(log_display, "Downloading APACHE_COMMONS_LANG3 library...\n");
  download_libraries((Library)APACHE_COMMONS_LANG3, "3.12.0");
  log_message(log_display, "Downloading GSON library...\n");
  download_libraries((Library)GSON, "2.7");
  log_message(log_display, "Downloading GUAVA library...\n");
  download_libraries((Library)GUAVA, "19.0");
  log_message(log_display, "Downloading ICU4J library...\n");
  download_libraries((Library)ICU4J, "65.1");
  log_message(log_display, "Downloading JOPT_SIMPLE library...\n");
  download_libraries((Library)JOPT_SIMPLE, "4.6");
  log_message(log_display, "Downloading LOG4J_CORE library...\n");
  download_libraries((Library)LOG4J_CORE, "2.17.2");
  log_message(log_display, "Downloading LOG4J_API library...\n");
  download_libraries((Library)LOG4J_API, "2.17.2");
  log_message(log_display, "Downloading PAULSCODE_CODECJORBIS library...\n");
  download_libraries((Library)PAULSCODE_CODECJORBIS, "20101023");
  log_message(log_display, "Downloading PAULSCODE_JAVA_SOUND library...\n");
  download_libraries((Library)PAULSCODE_JAVA_SOUND, "20101123");
  log_message(log_display, "Downloading PAULSCODE_LWJGL_OPENAL library...\n");
  download_libraries((Library)PAULSCODE_LWJGL_OPENAL, "0.0.1");
  log_message(log_display, "Downloading PAULSCODE_SOUND_SYSTEM library...\n");
  download_libraries((Library)PAULSCODE_SOUND_SYSTEM, "20120107");
  log_message(log_display, "Downloading NETTY_AIO library...\n\n");
  download_libraries((Library)NETTY_AIO, "4.0.56.Final");
  log_message(log_display, "Starting Minecraft game...\n");
  play_game(mc_version, mc_username, mc_gamedir, mc_class);
}

