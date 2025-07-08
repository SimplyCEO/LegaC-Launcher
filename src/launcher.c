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

/* Settings window */

char xms_string[16] = {'1','2','8',0};
char xmx_string[16] = {'1','0','2','4',0};

static void
on_settings_xms_changed(GtkWidget* widget, gpointer data)
{
  strcpy(xms_string, gtk_entry_get_text(GTK_ENTRY(widget)));
  application.settings.xms = atoi(xms_string);
}

static void
on_settings_xmx_changed(GtkWidget* widget, gpointer data)
{
  strcpy(xmx_string, gtk_entry_get_text(GTK_ENTRY(widget)));
  application.settings.xmx = atoi(xmx_string);
}

static void
on_settings_extra_arguments_changed(GtkWidget* widget, gpointer data)
{ strcpy(application.settings.extra_arguments, gtk_entry_get_text(GTK_ENTRY(widget))); }

static void
CreateSettingsWindow(gpointer data)
{
  GtkWidget* settings_window = CApplication.Create.Window("Launcher settings", 210, 70, GTK_WIN_POS_CENTER);
  GtkWidget* settings_vbox   = CApplication.Create.Box(settings_window, GTK_ORIENTATION_VERTICAL);
  { /* JavaVM Memory Management */
    GtkWidget* settings_memory_hbox   = CApplication.Create.Box(settings_vbox, GTK_ORIENTATION_HORIZONTAL);
    { GtkWidget* settings_memory_vbox_start = CApplication.Create.Box(settings_memory_hbox, GTK_ORIENTATION_VERTICAL);
      { /* Label */
        GtkWidget* settings_xms_label = CApplication.Create.Label("JavaVM Boot Memory", settings_memory_vbox_start, "start");
        /* Entry */
        GtkWidget* settings_xms = CApplication.Create.Entry("", settings_memory_vbox_start, "end");
                                  CApplication.Event.Align(settings_xms, "horizontal", GTK_ALIGN_START);
                                  CApplication.Event.Resize(settings_xms, 70, 35);
                                  CApplication.Event.Connect(settings_xms, "changed", on_settings_xms_changed, NULL);
                                  CApplication.Update.Entry.Text(settings_xms, xms_string);
      }
      CApplication.Event.Attach(settings_memory_vbox_start, settings_memory_hbox, "start", FALSE, FALSE, 0);

      GtkWidget* settings_memory_vbox_end = CApplication.Create.Box(settings_memory_hbox, GTK_ORIENTATION_VERTICAL);
      { /* Label */
        GtkWidget* settings_xmx_label = CApplication.Create.Label("JavaVM Max Memory", settings_memory_vbox_end, "start");
        /* Entry */
        GtkWidget* settings_xmx = CApplication.Create.Entry("", settings_memory_vbox_end, "end");
                                  CApplication.Event.Align(settings_xmx, "horizontal", GTK_ALIGN_START);
                                  CApplication.Event.Resize(settings_xmx, 70, 35);
                                  CApplication.Event.Connect(settings_xmx, "changed", on_settings_xmx_changed, NULL);
                                  CApplication.Update.Entry.Text(settings_xmx, xmx_string);
      }
      gtk_box_pack_end(GTK_BOX(settings_memory_hbox), settings_memory_vbox_end, FALSE, FALSE, 0);
    }
    CApplication.Event.Attach(settings_memory_hbox, settings_vbox, "start", FALSE, FALSE, 3);
    
    /* Extra arguments */
    GtkWidget* settings_extra_arguments_vbox = CApplication.Create.Box(settings_vbox, GTK_ORIENTATION_VERTICAL);
    { /* Label */
      GtkWidget* settings_extra_arguments_label = CApplication.Create.Label("Extra arguments", settings_extra_arguments_vbox, "start");
      /* Entry */
      GtkWidget* settings_extra_arguments = CApplication.Create.Entry("", settings_extra_arguments_vbox, "end");
                                            CApplication.Event.Align(settings_extra_arguments, "horizontal", GTK_ALIGN_START);
                                            CApplication.Event.Resize(settings_extra_arguments, 310, 35);
                                            CApplication.Event.Connect(settings_extra_arguments, "changed", on_settings_extra_arguments_changed, NULL);
                                            CApplication.Update.Entry.Text(settings_extra_arguments, application.settings.extra_arguments);
    }
    gtk_box_pack_end(GTK_BOX(settings_vbox), settings_extra_arguments_vbox, FALSE, FALSE, 3);
  }

  gtk_widget_show_all(settings_window);
}

/* Main window */

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
on_play_clicked(GtkWidget* widget, void* log_display)
{ GThread* thread = g_thread_new("initialise_game", (GThreadFunc)initialise_game, NULL); }

static void
on_settings_clicked(GtkWidget* widget, gpointer data)
{ GThread* thread = g_thread_new("CreateSettingsWindow", (GThreadFunc)CreateSettingsWindow, NULL); }

static void
InitialiseWindow(const char* window_name, const char* window_icon, const char* window_background, int size_x, int size_y, GtkWindowPosition window_position)
{
  window =  CApplication.Create.Window(window_name, size_x, size_y, window_position);
            CApplication.Icon.Window(window, window_icon);
            CApplication.Background.Window(window, window_background);
}

static void
CreateHeader(void)
{
  unsigned char useNewsPage = 1;
  char* NewsPageStatus = getenv("LEGAC_NEWS_PAGE");
  if (NewsPageStatus != NULL)
  { useNewsPage = atoi(NewsPageStatus); }

  vbox  = CApplication.Create.Box(window, GTK_ORIENTATION_VERTICAL);
  gtk_widget_set_vexpand(vbox, TRUE);

  if (useNewsPage)
  { /* Old Minecraft news */
    WebKitWebView* web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
    CApplication.Event.Attach(GTK_WIDGET(web_view), vbox, "start", TRUE, TRUE, 0);

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
  hbox = CApplication.Create.Box(window, GTK_ORIENTATION_HORIZONTAL);
         CApplication.Event.Attach(hbox, vbox, "end", FALSE, FALSE, 5);
  hgrid = gtk_grid_new();
          CApplication.Event.Attach(hgrid, hbox, "start", FALSE, FALSE, 0);
          gtk_grid_set_column_homogeneous(GTK_GRID(hgrid), TRUE);

  /* Game version */
  profile_box = CApplication.Create.Box(NULL, GTK_ORIENTATION_VERTICAL);
  { version_list = gtk_combo_box_text_new();
    { /* TODO: Get all files from internet and store somewhere in user configuration. */
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_list), "1.5.2");
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_list), "1.7.10");
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_list), "1.8.9");
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_list), "1.9");
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_list), "1.12.2");
      gtk_combo_box_set_active(GTK_COMBO_BOX(version_list), 3);
      CApplication.Event.Connect(version_list, "changed", on_version_choose, NULL);
    }
    CApplication.Event.Align(version_list, "horizontal", GTK_ALIGN_CENTER);
    CApplication.Event.Attach(version_list, profile_box, "start", FALSE, FALSE, 0);

    settings_button = gtk_button_new_with_label("Settings");
    { CApplication.Event.Connect(settings_button, "clicked", on_settings_clicked, NULL); }
    CApplication.Event.Align(settings_button, "horizontal", GTK_ALIGN_CENTER);
    CApplication.Event.Attach(settings_button, profile_box, "start", FALSE, FALSE, 0);
  }
  CApplication.Event.Align(profile_box, "horizontal", GTK_ALIGN_START);
  gtk_grid_attach(GTK_GRID(hgrid), profile_box, 0, 0, 1, 1);

  /* Download and/or Play */
  play_box = CApplication.Create.Box(NULL, GTK_ORIENTATION_VERTICAL);
  { play_button = gtk_button_new_with_label("Play");
    { CApplication.Event.Connect(play_button, "clicked", on_play_clicked, NULL); }
    CApplication.Event.Resize(play_button, 250, 100);
    CApplication.Event.Align(play_button, "horizontal", GTK_ALIGN_START);
    CApplication.Event.Attach(play_button, play_box, "start", FALSE, FALSE, 0);
  }
  gtk_grid_attach(GTK_GRID(hgrid), play_box, 1, 0, 1, 1);

  /* User profile and Help page */
  info_box = CApplication.Create.Box(hbox, GTK_ORIENTATION_VERTICAL);
  { user_entry = CApplication.Create.Entry("Username", info_box, "start");
                 CApplication.Event.Align(user_entry, "horizontal", GTK_ALIGN_START);
                 CApplication.Event.Resize(user_entry, 70, 35);
                 CApplication.Event.Connect(user_entry, "changed", on_username_changed, user_text);

    repository = gtk_button_new_with_label("Help");
    { CApplication.Event.Connect(repository, "clicked", on_click_open_website, application.help.url); }
    CApplication.Event.Resize(repository, 140, 35);
    CApplication.Event.Align(repository, "horizontal", GTK_ALIGN_CENTER);
    CApplication.Event.Attach(repository, info_box, "start", FALSE, FALSE, 3);
  }
  CApplication.Event.Align(info_box, "horizontal", GTK_ALIGN_END);
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

