#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "args.h"
#include "Application.h"
#include "Downloader.h"

void
play_game(const char* mc_dir, const char* mc_version, const char* mc_username)
{
  char java_buffer[2048] = {0};

  char mc_class[64] = {0};

  if (!strncmp(mc_version, "1.9", 3))
  { strcpy(mc_class, "net.minecraft.client.main.Main"); }

  sprintf(java_buffer, "java -Xms128M -Xmx1024M -Djava.library.path='%s/bin/natives' -cp '%s/bin/*' %s --username %s --version %s --accessToken 0 --userProperties {} --gameDir %s --assetsDir ./assets --width 800 --height 600", mc_dir, mc_dir, mc_class, mc_username, mc_version, mc_dir);

  system(java_buffer);
}

void
on_button_clicked(GtkWidget *widget, gpointer data)
{
  GtkTextBuffer *buffer = GTK_TEXT_BUFFER(data);
  GtkTextIter iter;

  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, "Downloading Minecraft game...\n", -1);
  gtk_main_iteration_do(FALSE);
  download_game("1.9", 1);

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
  play_game("./instances/1.9/minecraft", "1.9", "legacy_launcher");
}

int
main(int argc, char* argv[])
{
  CApplication.Initialise(argc, argv);

  GtkWidget* main_window = CApplication.Window.Create(application.name, 700, 490, GTK_WIN_POS_CENTER);

  GtkWidget* vertical_box  = CApplication.Box.Create(main_window, GTK_ORIENTATION_VERTICAL);

  GtkWidget *text_view = gtk_text_view_new();
  gtk_box_pack_start(GTK_BOX(vertical_box), text_view, TRUE, TRUE, 0);

  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  GtkWidget* horizontal_box = CApplication.Box.Create(main_window, GTK_ORIENTATION_HORIZONTAL);
  gtk_box_pack_end(GTK_BOX(vertical_box), horizontal_box, FALSE, FALSE, 0);

  GtkWidget *button = gtk_button_new_with_label("Play");
  CApplication.Box.Resize(button, 140, 70);
  gtk_box_pack_start(GTK_BOX(horizontal_box), button, TRUE, FALSE, 0);
  gtk_widget_set_halign(button, GTK_ALIGN_CENTER);

  g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), buffer);

  gtk_widget_show_all(main_window);
  CApplication.Loop();

  return 0;
}

