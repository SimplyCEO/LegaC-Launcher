#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "args.h"
#include "settings.h"
#include "Application.h"
#include "Downloader.h"

MinecraftSettings mc = {{1,9,0},"",{{0},{'.','/','a','s','s','e','t','s'}}};

void
play_game(const char* mc_version, const char* mc_username, const char* mc_gamedir, const char* mc_class)
{
  char java_buffer[2048] = {0};
  sprintf(java_buffer, "java -Xms128M -Xmx1024M -Djava.library.path='%s/bin/natives' -cp '%s/bin/*' %s --username %s --version %s --accessToken 0 --userProperties {} --gameDir %s --assetsDir ./assets --width 800 --height 600", mc_gamedir, mc_gamedir, mc_class, mc_username, mc_version, mc_gamedir);
  /* printf("JAVA COMMAND: %s\n", java_buffer); */
  system(java_buffer);
}

void
on_version_choose(GtkWidget* widget, gpointer data)
{
  gint version_id = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
  switch (version_id)
  { case 0:  mc.version.major = 1; mc.version.minor = 5,  mc.version.patch = 0;  break;
    case 1:  mc.version.major = 1; mc.version.minor = 5,  mc.version.patch = 2;  break;
    case 2:  mc.version.major = 1; mc.version.minor = 6,  mc.version.patch = 0;  break;
    case 3:  mc.version.major = 1; mc.version.minor = 7,  mc.version.patch = 0;  break;
    case 4:  mc.version.major = 1; mc.version.minor = 7,  mc.version.patch = 10; break;
    case 5:  mc.version.major = 1; mc.version.minor = 8,  mc.version.patch = 0;  break;
    case 6:  mc.version.major = 1; mc.version.minor = 8,  mc.version.patch = 9;  break;
    case 7:  mc.version.major = 1; mc.version.minor = 9,  mc.version.patch = 0;  break;
    case 8:  mc.version.major = 1; mc.version.minor = 10, mc.version.patch = 0;  break;
    case 9:  mc.version.major = 1; mc.version.minor = 11, mc.version.patch = 0;  break;
    case 10: mc.version.major = 1; mc.version.minor = 12, mc.version.patch = 0;  break;
    default: break;
  }
}

void
on_click_open_website(GtkWidget *widget, gpointer data)
{
  const gchar* url = (const gchar*)data;
  gtk_show_uri_on_window(NULL, url, GDK_CURRENT_TIME, NULL);
}

void
on_username_changed(GtkWidget *widget, gpointer data)
{
  const gchar* username = gtk_entry_get_text(GTK_ENTRY(widget));
  strcpy(mc.username, username);
}

void
on_button_clicked(GtkWidget *widget, gpointer data)
{
  GtkTextBuffer *buffer = GTK_TEXT_BUFFER(data);
  GtkTextIter iter;

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
  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading Minecraft game...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_game(mc_version, 1);

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading LWJGL library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)LWJGL, "2.9.3");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading AUTHLIB library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)AUTHLIB, "1.5.25");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading APACHE_COMMONS_IO library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)APACHE_COMMONS_IO, "2.5");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading APACHE_COMMONS_LANG3 library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)APACHE_COMMONS_LANG3, "3.12.0");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading GSON library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)GSON, "2.7");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading GUAVA library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)GUAVA, "19.0");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading ICU4J library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)ICU4J, "65.1");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading JOPT_SIMPLE library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)JOPT_SIMPLE, "4.6");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading LOG4J_CORE library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)LOG4J_CORE, "2.17.2");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading LOG4J_API library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)LOG4J_API, "2.17.2");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading PAULSCODE_CODECJORBIS library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)PAULSCODE_CODECJORBIS, "20101023");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading PAULSCODE_JAVA_SOUND library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)PAULSCODE_JAVA_SOUND, "20101123");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading PAULSCODE_LWJGL_OPENAL library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)PAULSCODE_LWJGL_OPENAL, "0.0.1");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading PAULSCODE_SOUND_SYSTEM library...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)PAULSCODE_SOUND_SYSTEM, "20120107");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading NETTY_AIO library...\n\n", -1);
  gtk_main_iteration_do(FALSE);
  download_libraries((Library)NETTY_AIO, "4.0.56.Final");

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Starting Minecraft game...\n", -1);
  gtk_main_iteration_do(FALSE);
  play_game(mc_version, mc_username, mc_gamedir, mc_class);
}

int
main(int argc, char* argv[])
{
  /* /------------------\
   * |                  |
   * |      LOGGER      |
   * |                  |
   * \------------------/
   * | VER |      | USR |
   * |     | PLAY |     |
   * |     |      | HLP |
   * \------------------/
   */
  GtkWidget *window,
            *vbox,
            *logger,
            *hbox, *profilebox, *infobox,
            *version_entry, *version_anchor_top, *play_button, *user_entry, *repository;
  GtkTextBuffer *logger_text, *user_text;
  CApplication.Initialise(argc, argv);

  window = CApplication.Window.Create(application.name, 700, 490, GTK_WIN_POS_CENTER);

  /* Header container */
  vbox  = CApplication.Box.Create(window, GTK_ORIENTATION_VERTICAL);

  /* Info logger */
  logger = gtk_text_view_new();
  gtk_box_pack_start(GTK_BOX(vbox), logger, TRUE, TRUE, 0);
  logger_text = gtk_text_view_get_buffer(GTK_TEXT_VIEW(logger));

  /* Docker container */
  hbox = CApplication.Box.Create(window, GTK_ORIENTATION_HORIZONTAL);
  gtk_box_pack_end(GTK_BOX(vbox), hbox, FALSE, FALSE, 3);

  /* Game version - Left anchor */
  profilebox = CApplication.Box.Create(NULL, GTK_ORIENTATION_VERTICAL);
  gtk_box_pack_start(GTK_BOX(hbox), profilebox, TRUE, FALSE, 0);

  version_entry = gtk_combo_box_text_new();
  /* TODO: Get all files from internet and store somewhere in user configuration. */
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.5");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.5.2");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.6");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.7");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.7.10");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.8");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.8.9");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.9");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.10");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.11");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.12");
  gtk_combo_box_set_active(GTK_COMBO_BOX(version_entry), 7);
  CApplication.Box.Resize(version_entry, 70, 35);
  gtk_box_pack_start(GTK_BOX(profilebox), version_entry, TRUE, FALSE, 3);
  gtk_widget_set_halign(version_entry, GTK_ALIGN_CENTER);
  g_signal_connect(version_entry, "changed", G_CALLBACK(on_version_choose), NULL);
  /* Extra padding */
  version_anchor_top = CApplication.Box.Create(NULL, GTK_ORIENTATION_VERTICAL);
  gtk_box_pack_start(GTK_BOX(profilebox), version_anchor_top, TRUE, FALSE, 3);
  gtk_widget_set_halign(version_anchor_top, GTK_ALIGN_END);

  /* Download and/or Play - Middle anchor */
  play_button = gtk_button_new_with_label("Play");
  CApplication.Box.Resize(play_button, 210, 70);
  gtk_box_pack_start(GTK_BOX(hbox), play_button, TRUE, FALSE, 3);
  gtk_widget_set_halign(play_button, GTK_ALIGN_CENTER);
  g_signal_connect(play_button, "clicked", G_CALLBACK(on_button_clicked), logger_text);

  /* User profile and help */
  infobox = CApplication.Box.Create(hbox, GTK_ORIENTATION_VERTICAL);
  gtk_box_pack_end(GTK_BOX(hbox), infobox, FALSE, FALSE, 0);

  user_entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(user_entry), "Username");
  CApplication.Box.Resize(user_entry, 70, 35);
  gtk_box_pack_start(GTK_BOX(infobox), user_entry, TRUE, FALSE, 3);
  gtk_widget_set_halign(user_entry, GTK_ALIGN_START);
  g_signal_connect(user_entry, "changed", G_CALLBACK(on_username_changed), user_text);

  repository = gtk_button_new_with_label("LegaC Help");
  CApplication.Box.Resize(repository, 140, 35);
  gtk_box_pack_start(GTK_BOX(infobox), repository, TRUE, FALSE, 3);
  gtk_widget_set_halign(repository, GTK_ALIGN_CENTER);
  g_signal_connect(repository, "clicked", G_CALLBACK(on_click_open_website), "https://gitlab.com/SimplyCEO/LegaC-Launcher/-/issues");

  gtk_widget_show_all(window);
  CApplication.Loop();

  return 0;
}

