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

unsigned char
download_library(Library library)
{
  char* version = NULL;

  switch (library)
  {
    case LWJGL:                   version = "2.9.3";            break;
    case AUTHLIB:                 version = "1.5.25";           break;
    case APACHE_COMMONS_IO:       version = "2.5";              break;
    case APACHE_COMMONS_LANG3:    version = "3.12.0";           break;
    case GSON:                    version = "2.7";              break;
    case GUAVA:                   version = "19.0";             break;
    case ICU4J:                   version = "65.1";             break;
    case JOPT_SIMPLE:             version = "4.6";              break;
    case LOG4J_CORE:              version = "2.0-beta9-fixed";  break;
    case LOG4J_API:               version = "2.0-beta9-fixed";  break;
    case PAULSCODE_CODECJORBIS:   version = "20101023";         break;
    case PAULSCODE_CODECWAV:      version = "20101023";         break;
    case PAULSCODE_JAVA_SOUND:    version = "20101123";         break;
    case PAULSCODE_LWJGL_OPENAL:  version = "20100824";         break;
    case PAULSCODE_SOUND_SYSTEM:  version = "20120107";         break;
    case NETTY_AIO:               version = "4.0.56.Final";     break;
    default: fprintf(stderr, "Unknown library.\n"); return 1;
  }

  download_libraries(library, version);
  dl_current++;

  return 0;
}

void
initialise_game(gpointer data)
{
  LogDisplay* log_display = (LogDisplay*)data;

  /* Build game configuration */

  /* Version string */
  char mc_version[12] = {0};
  if (mc.version.patch > 0)
  { sprintf(mc_version, "%hhu.%hhu.%hhu", mc.version.major, mc.version.minor, mc.version.patch); }
  else
  { sprintf(mc_version, "%hhu.%hhu", mc.version.major, mc.version.minor); }

  /* Username string default to `LegaC_user` */
  char mc_username[16] = {0};
  if (mc.username[0] != '\0')
  { strncpy(mc_username, mc.username, 16); }
  else
  { strcpy(mc_username, "LegaC_user");
    strcpy(mc.username, mc_username);
  }

  /* instances/VERSION/minecraft game directory */
  char mc_gamedir[128] = {0};
  sprintf(mc_gamedir, "./instances/%s/minecraft", mc_version);
  strcpy(mc.directory.game, mc_gamedir);

  /* Necessary class name */
  char mc_class[32] = {0};
  if ((mc.version.major == 1) & (mc.version.minor > 5))
  { strcpy(mc_class, "net.minecraft.client.main.Main"); }
  else
  { strcpy(mc_class, "net.minecraft.client.Minecraft"); }

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

  unsigned char i = 0;
  for (i=0; i<=(int)dl_total; i++)
  { download_library((Library)i); }

  log_message(log_display, "Starting Minecraft game...\n");
  gtk_widget_destroy(GTK_WIDGET(progress_window));

  play_game(mc_version, mc_username, mc_gamedir, mc_class);
}

