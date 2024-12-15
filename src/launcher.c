#include <stdio.h>

#include "data/settings.h"

#include "game.h"
#include "Application.h"
#include "FileManager.h"
#include "Internet.h"

LogDisplay *logger = NULL;

static void
on_version_choose(GtkWidget* widget, gpointer data)
{
  gint version_id = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
  switch (version_id)
  { case 0: mc.version.major = 1; mc.version.minor = 5,  mc.version.patch = 2;  break;
    case 1: mc.version.major = 1; mc.version.minor = 7,  mc.version.patch = 10; break;
    case 2: mc.version.major = 1; mc.version.minor = 8,  mc.version.patch = 9;  break;
    case 3: mc.version.major = 1; mc.version.minor = 9,  mc.version.patch = 0;  break;
    case 4: mc.version.major = 1; mc.version.minor = 12, mc.version.patch = 0;  break;
    default: break;
  }
}

static void
on_click_open_website(GtkWidget* widget, gpointer data)
{
  const gchar* url = (const gchar*)data;
  gtk_show_uri_on_window(NULL, url, GDK_CURRENT_TIME, NULL);
}

static void
on_username_changed(GtkWidget* widget, gpointer data)
{
  const gchar* username = gtk_entry_get_text(GTK_ENTRY(widget));
  strcpy(mc.username, username);
}

static void
onLogWindow(GtkWidget* widget, LogDisplay* log_display)
{
  GtkTextBuffer *buffer;
  GtkTextIter start_iter, end_iter;
  gchar* message;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log_display->text_view));
  gtk_text_buffer_get_bounds(buffer, &start_iter, &end_iter);
  gtk_text_buffer_delete(buffer, &start_iter, &end_iter);
  
  while ((message = g_async_queue_try_pop(log_display->queue)))
  { g_free(message); }

  /* GThread* thread = g_thread_new("gameLogWindow", (GThreadFunc)gameLogWindow, log_display); */
}

static void
on_play_clicked(GtkWidget* widget, LogDisplay* log_display)
{ GThread* thread = g_thread_new("initialise_game", (GThreadFunc)initialise_game, NULL); }

void
startApplication(int argc, char** argv)
{
  CApplication.Initialise(argc, argv);

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
  GtkWidget *window = NULL;
  GtkWidget *vbox   = NULL;
  GtkWidget *hbox   = NULL,
              *profilebox = NULL,
              *infobox    = NULL,
                *version_entry      = NULL,
                *version_anchor_top = NULL,
                *play_button        = NULL,
                *user_entry         = NULL,
                *repository         = NULL;

  GtkTextBuffer *logger_text  = NULL,
                *user_text    = NULL;

  /* TODO: Identify the size of the monitor to display a good resolution. */
  window = CApplication.Window.Create(application.name, 800, 600, GTK_WIN_POS_CENTER);
           CApplication.Window.SetIcon(window, "./assets/icon.png");
           CApplication.Window.SetBackground(window, "./assets/background.png");

  /* Header container */
  vbox  = CApplication.Box.Create(window, GTK_ORIENTATION_VERTICAL);

  /* Old Minecraft news */
  WebKitWebView* web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
  gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(web_view), TRUE, TRUE, 0);

  if (CFileManager.Validate("./assets/launcher_news.html") == 0)
  { CInternet.HandleData("./assets/launcher_news.html", "https://mcupdate.tumblr.com"); }

  char* html_file = CFileManager.ReadFileFromPath("./assets/launcher_news.html");
  webkit_web_view_load_html(web_view, html_file, NULL);
  free(html_file);

  /* Info logger */
  /* logger = CApplication.Text.Logger(vbox); */

  /* Docker container */
  hbox = CApplication.Box.Create(window, GTK_ORIENTATION_HORIZONTAL);
  gtk_box_pack_end(GTK_BOX(vbox), hbox, FALSE, FALSE, 3);

  /* Game version - Left anchor */
  profilebox = CApplication.Box.Create(NULL, GTK_ORIENTATION_VERTICAL);
  gtk_box_pack_start(GTK_BOX(hbox), profilebox, TRUE, FALSE, 0);

  version_entry = gtk_combo_box_text_new();
  /* TODO: Get all files from internet and store somewhere in user configuration. */
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.5.2");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.7.10");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.8.9");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.9");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_entry), "1.12.2");
  gtk_combo_box_set_active(GTK_COMBO_BOX(version_entry), 3);
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
  g_signal_connect(play_button, "clicked", G_CALLBACK(on_play_clicked), NULL);

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
}
