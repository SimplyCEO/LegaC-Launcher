#include <string.h>
#include <stdlib.h>

#include "data/settings.h"
#include "data/Downloader.h"

#include "args.h"
#include "game.h"
#include "Application.h"

void
play_game(const char* mc_version, const char* mc_username, const char* mc_gamedir, const char* mc_class)
{
  char java_buffer[2048] = {0};
  sprintf(java_buffer, "java -Xms128M -Xmx1024M");
  #if !defined(_WIN32)
    sprintf(java_buffer, "%s -Dorg.lwjgl.openal.libname=/lib/libopenal.so", java_buffer);
  #endif
  sprintf(java_buffer, "%s -Djava.library.path='%s/bin/natives' -cp '%s/bin/*' %s --username %s --version %s --accessToken 0 --userProperties {} --gameDir %s --assetsDir %s/assets --assetIndex %s --width 800 --height 600", java_buffer, mc_gamedir, mc_gamedir, mc_class, mc_username, mc_version, mc_gamedir, mc_gamedir, mc_version);
  /* printf("JAVA COMMAND: %s\n", java_buffer); */
  system(java_buffer);
}

void
log_message(LogDisplay *logger, const gchar *message)
{ g_async_queue_push(logger->queue, g_strdup(message)); }

gint dl_total = 0, dl_current = 0;
void
update_progress_bar(gpointer progress_bar)
{
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), (gdouble) dl_current / dl_total);
  /* I do have to say, I don't know why this is needed... Does not have any fucking sense! WHAT??? */
  if (dl_current == dl_total)
  { gtk_widget_destroy(GTK_WIDGET(progress_bar)); }
}

void
initialise_game(gpointer data)
{
  LogDisplay* log_display = (LogDisplay*)data;

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

  /* Start progress bar */
  dl_total = (Library)LIBRARY_LAST-1;
  GtkWidget* progress_window  = CApplication.Window.Create("Downloading dependencies...", 350, 70, GTK_WIN_POS_CENTER);
  GtkWidget* progress_bar     = gtk_progress_bar_new();
  gtk_container_add(GTK_CONTAINER(progress_window), progress_bar);
  gtk_widget_show_all(progress_window);
  g_idle_add((GSourceFunc)update_progress_bar, progress_bar);

  /* Download dependencies and run game */
  log_message(log_display, "Downloading Minecraft game...\n");
  download_game(mc_version, 1);
  dl_current++;

  log_message(log_display, "Downloading LWJGL library...\n");
  download_libraries((Library)LWJGL, "2.9.3");
  dl_current++;

  log_message(log_display, "Downloading AUTHLIB library...\n");
  download_libraries((Library)AUTHLIB, "1.5.25");
  dl_current++;

  log_message(log_display, "Downloading APACHE_COMMONS_IO library...\n");
  download_libraries((Library)APACHE_COMMONS_IO, "2.5");
  dl_current++;

  log_message(log_display, "Downloading APACHE_COMMONS_LANG3 library...\n");
  download_libraries((Library)APACHE_COMMONS_LANG3, "3.12.0");
  dl_current++;

  log_message(log_display, "Downloading GSON library...\n");
  download_libraries((Library)GSON, "2.7");
  dl_current++;

  log_message(log_display, "Downloading GUAVA library...\n");
  download_libraries((Library)GUAVA, "19.0");
  dl_current++;

  log_message(log_display, "Downloading ICU4J library...\n");
  download_libraries((Library)ICU4J, "65.1");
  dl_current++;

  log_message(log_display, "Downloading JOPT_SIMPLE library...\n");
  download_libraries((Library)JOPT_SIMPLE, "4.6");
  dl_current++;

  log_message(log_display, "Downloading LOG4J_CORE library...\n");
  download_libraries((Library)LOG4J_CORE, "2.0-beta9-fixed");
  dl_current++;

  log_message(log_display, "Downloading LOG4J_API library...\n");
  download_libraries((Library)LOG4J_API, "2.0-beta9-fixed");
  dl_current++;

  log_message(log_display, "Downloading PAULSCODE_CODECJORBIS library...\n");
  download_libraries((Library)PAULSCODE_CODECJORBIS, "20101023");
  dl_current++;

  log_message(log_display, "Downloading PAULSCODE_CODECWAV library...\n");
  download_libraries((Library)PAULSCODE_CODECWAV, "20101023");
  dl_current++;

  log_message(log_display, "Downloading PAULSCODE_JAVA_SOUND library...\n");
  download_libraries((Library)PAULSCODE_JAVA_SOUND, "20101123");
  dl_current++;

  log_message(log_display, "Downloading PAULSCODE_LWJGL_OPENAL library...\n");
  download_libraries((Library)PAULSCODE_LWJGL_OPENAL, "20100824");
  dl_current++;

  log_message(log_display, "Downloading PAULSCODE_SOUND_SYSTEM library...\n");
  download_libraries((Library)PAULSCODE_SOUND_SYSTEM, "20120107");
  dl_current++;

  log_message(log_display, "Downloading NETTY_AIO library...\n\n");
  download_libraries((Library)NETTY_AIO, "4.0.56.Final");
  dl_current++;

  log_message(log_display, "Starting Minecraft game...\n");
  gtk_widget_destroy(GTK_WIDGET(progress_window));

  play_game(mc_version, mc_username, mc_gamedir, mc_class);
}

