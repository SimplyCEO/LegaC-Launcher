#include <stdio.h>
#include <stdlib.h>

#include "data/settings.h"
#include "data/Application.h"

#include "game.h"
#include "Application.h"
#include "FileManager.h"
#include "Internet.h"

GtkWidget *window = NULL;
GtkWidget *vbox   = NULL;
GtkWidget *hbox   = NULL,
            *hgrid  = NULL,
              *profile_box      = NULL,
                *version_list     = NULL,
                *settings_button  = NULL,
              *play_box         = NULL,
                *play_button      = NULL,
              *info_box         = NULL,
                *user_entry       = NULL,
                *repository       = NULL;

GtkTextBuffer *logger_text  = NULL,
              *user_text    = NULL;

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



static void
InitialiseWindow(const char* window_name, const char* window_icon, const char* window_background, int size_x, int size_y, GtkWindowPosition window_position)
{
  window =  CApplication.Window.Create(window_name, size_x, size_y, window_position);
            CApplication.Window.SetIcon(window, window_icon);
            CApplication.Window.SetBackground(window, window_background);
}

static void
CreateHeader(void)
{
  unsigned char useNewsPage = 1;
  char* NewsPageStatus = getenv("LEGAC_NEWS_PAGE");
  if (NewsPageStatus != NULL)
  { useNewsPage = atoi(NewsPageStatus); }

  vbox  = CApplication.Box.Create(window, GTK_ORIENTATION_VERTICAL);
  gtk_widget_set_vexpand(vbox, TRUE);

  if (useNewsPage)
  { /* Old Minecraft news */
    WebKitWebView* web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(web_view), TRUE, TRUE, 0);

    if (CFileManager.Validate(application.news.html) == 0)
    { CInternet.HandleData(application.news.html, application.news.url); }

    char* html_file = CFileManager.ReadFileFromPath(application.news.html);
    webkit_web_view_load_html(web_view, html_file, NULL);
    free(html_file);
  }

  /* Info logger */
  /* logger = CApplication.Text.Logger(vbox); */
}

static void
CreateDocker(void)
{
  /* Docker container grid */
  hbox = CApplication.Box.Create(window, GTK_ORIENTATION_HORIZONTAL);
  gtk_box_pack_end(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);
  hgrid = gtk_grid_new();
  gtk_box_pack_start(GTK_BOX(hbox), hgrid, FALSE, FALSE, 0);
  gtk_grid_set_column_homogeneous(GTK_GRID(hgrid), TRUE);

  /* Game version */
  profile_box = CApplication.Box.Create(NULL, GTK_ORIENTATION_VERTICAL);
  { version_list = gtk_combo_box_text_new();
    { /* TODO: Get all files from internet and store somewhere in user configuration. */
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_list), "1.5.2");
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_list), "1.7.10");
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_list), "1.8.9");
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_list), "1.9");
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_list), "1.12.2");
      gtk_combo_box_set_active(GTK_COMBO_BOX(version_list), 3);
      g_signal_connect(version_list, "changed", G_CALLBACK(on_version_choose), NULL);
    }
    gtk_widget_set_halign(version_list, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(profile_box), version_list, FALSE, FALSE, 0);

    settings_button = gtk_button_new_with_label("Settings");
    gtk_widget_set_halign(settings_button, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(profile_box), settings_button, FALSE, FALSE, 0);
  }
  gtk_widget_set_halign(profile_box, GTK_ALIGN_START);
  gtk_grid_attach(GTK_GRID(hgrid), profile_box, 0, 0, 1, 1);

  /* Download and/or Play */
  play_box = CApplication.Box.Create(NULL, GTK_ORIENTATION_VERTICAL);
  { play_button = gtk_button_new_with_label("Play");
    { g_signal_connect(play_button, "clicked", G_CALLBACK(on_play_clicked), NULL); }
    CApplication.Box.Resize(play_button, 250, 100);
    gtk_widget_set_halign(play_button, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(play_box), play_button, FALSE, FALSE, 0);
  }
  gtk_grid_attach(GTK_GRID(hgrid), play_box, 1, 0, 1, 1);

  /* User profile and Help page */
  info_box = CApplication.Box.Create(hbox, GTK_ORIENTATION_VERTICAL);
  { user_entry = gtk_entry_new();
    { gtk_entry_set_placeholder_text(GTK_ENTRY(user_entry), "Username");
      g_signal_connect(user_entry, "changed", G_CALLBACK(on_username_changed), user_text);
    }
    CApplication.Box.Resize(user_entry, 70, 35);
    gtk_widget_set_halign(user_entry, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(info_box), user_entry, FALSE, FALSE, 3);

    repository = gtk_button_new_with_label("LegaC Help");
    { g_signal_connect(repository, "clicked", G_CALLBACK(on_click_open_website), application.help.url); }
    CApplication.Box.Resize(repository, 140, 35);
    gtk_widget_set_halign(repository, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(info_box), repository, FALSE, FALSE, 3);
  }
  gtk_widget_set_halign(info_box, GTK_ALIGN_END);
  gtk_widget_set_hexpand(info_box, TRUE);
  gtk_grid_attach(GTK_GRID(hgrid), info_box, 2, 0, 1, 1);
}

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

void
startApplication(int argc, char** argv)
{
  CApplication.Initialise(argc, argv);
  CApplication.SetupPath();

  /* TODO: Identify the size of the monitor to display a good resolution. */
  InitialiseWindow(application.name, application.icon, application.background, 800, 600, GTK_WIN_POS_CENTER);
  CreateHeader();
  CreateDocker();

  gtk_widget_show_all(window);

  CApplication.Loop();
}

