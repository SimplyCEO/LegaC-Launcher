#include <stdlib.h>

#include "data/Application.h"

#include "FileManager.h"

ApplicationData application =
{
  {0}, "LegaC Launcher", {0}, {0}, "v0.0.8",
  { 128, 1024, {0}, { 800, 600 } },
  { {0}, "https://mcupdate.tumblr.com" },
  { {0}, "https://gitlab.com/SimplyCEO/LegaC-Launcher/-/issues" }
};

unsigned char isMainWindowCreated = 0;

/*
 ==============================================================================================================
 */

static void
on_window_destroy(GtkWidget *widget, gpointer data)
{ gtk_main_quit(); }

static void
set_widget_size(GtkWidget* widget, int size_x, int size_y)
{ gtk_widget_set_size_request(widget, size_x, size_y); }

static gboolean
process_log_messages(LogDisplay *log_display)
{
  gchar *message;
  GtkTextBuffer *buffer;
  GtkTextIter end_iter;

  while ((message = g_async_queue_try_pop(log_display->queue)))
  { buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log_display->text_view));
    gtk_text_buffer_get_end_iter(buffer, &end_iter);
    gtk_text_buffer_insert(buffer, &end_iter, message, -1);
    g_free(message);
  }

  return TRUE;
}

/*
 ==============================================================================================================
 */

void
_A_Background_Window(GtkWidget* window, const char* filepath)
{
  GtkCssProvider* provider = gtk_css_provider_new();

  gchar* css = g_strdup_printf("window { background-image: url('%s'); background-size: 96px 96px; background-repeat: repeat; }", filepath);
  gtk_css_provider_load_from_data(provider, css, -1, NULL);

  GtkStyleContext* style_context = gtk_widget_get_style_context(window);
  gtk_style_context_add_provider(style_context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  g_free(css);
  g_object_unref(provider);
}

/*
 ==============================================================================================================
 */

GtkWidget*
_A_Create_Box(GtkWidget* container, GtkOrientation orientation)
{
  GtkWidget *box = gtk_box_new(orientation, 0);
  if (container != NULL)
  { gtk_container_add(GTK_CONTAINER(container), box); }

  return box;
}

GtkWidget*
_A_Create_Entry(const char* description, GtkWidget* container, const char* position)
{
  GtkWidget* entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), description);

  switch (position[0])
  { case 'E': case 'e': gtk_box_pack_end(GTK_BOX(container), entry, FALSE, FALSE, 0); break;
    default: gtk_box_pack_start(GTK_BOX(container), entry, FALSE, FALSE, 0); break;
  }

  return entry;
}

GtkWidget*
_A_Create_Label(const char* title, GtkWidget* container, const char* position)
{
  GtkWidget* label = gtk_label_new(title);

  switch (position[0])
  { case 'E': case 'e': gtk_box_pack_end(GTK_BOX(container), label, FALSE, FALSE, 0); break;
    default: gtk_box_pack_start(GTK_BOX(container), label, FALSE, FALSE, 0); break;
  }

  return label;
}

GtkWidget*
_A_Create_ProgressBar(GtkWidget* container, const char* position)
{
  GtkWidget* progress_bar = gtk_progress_bar_new();

  switch (position[0])
  { case 'E': case 'e': gtk_box_pack_end(GTK_BOX(container), progress_bar, FALSE, TRUE, 0); break;
    default: gtk_box_pack_start(GTK_BOX(container), progress_bar, FALSE, TRUE, 0); break;
  }

  return progress_bar;
}

GtkWidget*
_A_Create_Window(const char* window_name, int size_x, int size_y, GtkWindowPosition window_position)
{
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_title(GTK_WINDOW(window), window_name);
  gtk_window_set_default_size(GTK_WINDOW(window), size_x, size_y);
  gtk_window_set_position(GTK_WINDOW(window), window_position);
  gtk_container_set_border_width(GTK_CONTAINER(window), 0);

  /* Destroy window after quitting application. */  
  if (isMainWindowCreated == 0)
  { g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);
    isMainWindowCreated = 1;
  }

  return window;
}

/*
 ==============================================================================================================
 */

void
_A_Event_Align(GtkWidget* widget, const char* position, GtkAlign align)
{
  switch (position[0])
  { case 'V': case 'v': gtk_widget_set_valign(widget, align); break;
    default: gtk_widget_set_halign(widget, align); break;
  }
}

void
_A_Event_Attach(GtkWidget* widget, GtkWidget* container, const char* position, gboolean h_expand, gboolean v_expand, unsigned short padding)
{
  switch (position[0])
  { case 'E': case 'e': gtk_box_pack_end(GTK_BOX(container), widget, h_expand, v_expand, padding); break;
    default: gtk_box_pack_start(GTK_BOX(container), widget, h_expand, v_expand, padding); break;
  }
}

void
_A_Event_Connect(GtkWidget* widget, const char* mode, void (*function)(GtkWidget*,gpointer), gpointer data)
{
  g_signal_connect(widget, mode, G_CALLBACK(function), data);
}

void
_A_Event_Resize(GtkWidget* widget, int size_x, int size_y)
{
  set_widget_size(widget, size_x, size_y);
}

/*
 ==============================================================================================================
 */

void
_A_Icon_Window(GtkWidget* window, const char* filepath)
{
  GdkPixbuf* icon = gdk_pixbuf_new_from_file(filepath, NULL);
  gtk_window_set_icon(GTK_WINDOW(window), icon);
  g_object_unref(icon);
}

/*
 ==============================================================================================================
 */

void
_A_Update_Entry_Description(GtkWidget* widget, const char* description)
{
  gtk_entry_set_placeholder_text(GTK_ENTRY(widget), description);
}

void
_A_Update_Entry_Text(GtkWidget* widget, const char* definition)
{
  gtk_entry_set_text(GTK_ENTRY(widget), definition);
}

/*
 --------------------------------------------------------------------------------------------------------------
 */

void
_A_Update_Label_Text(GtkWidget* widget, const char* title)
{
  gtk_label_set_text(GTK_LABEL(widget), title);
}

/*
 --------------------------------------------------------------------------------------------------------------
 */

LogDisplay*
_A_Update_Logger(GtkWidget* widget)
{
  LogDisplay* log_display;
  GtkWidget* scrolled_window;
  GtkTextBuffer* buffer;

  log_display = g_malloc(sizeof(LogDisplay));

  log_display->text_view = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(log_display->text_view), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(log_display->text_view), FALSE);

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log_display->text_view));
  gtk_text_buffer_set_text(buffer, "", -1);

  scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scrolled_window), log_display->text_view);
  gtk_box_pack_start(GTK_BOX(widget), scrolled_window, TRUE, TRUE, 0);

  log_display->queue = g_async_queue_new();
  log_display->source_id = g_timeout_add(100, (GSourceFunc)process_log_messages, log_display);

  return log_display;
}

/*
 ==============================================================================================================
 */

/*
 ==============================================================================================================
 */

void
_A_Initialise(int argc, char* argv[])
{
  gtk_init(&argc, &argv);
}

void
_A_Loop(void)
{
  gtk_main();
}

void
_A_SetupPath(void)
{
  char* user = NULL;
  user = getenv("HOME");

  char* path = malloc(strlen(user)+29+1);
  strcpy(path, user);
  strcat(path, "/.local/share/legac_launcher");

  char* system_cmd = malloc(strlen(path)+strlen("mkdir -p ")+strlen("/assets")+1);
  strcpy(system_cmd, "mkdir -p ");
  strcat(system_cmd, path);
  strcat(system_cmd, "/assets");
  system(system_cmd);
  free(system_cmd);

  strcpy(application.path, path);

  char* icon_path = malloc(strlen(path)+strlen("/assets/icon.png")+1);
  strcpy(icon_path, path);
  strcat(icon_path, "/assets/icon.png");
  strcpy(application.icon, icon_path);
  free(icon_path);

  char* background_path = malloc(strlen(path)+strlen("/assets/background.png")+1);
  strcpy(background_path, path);
  strcat(background_path, "/assets/background.png");
  strcpy(application.background, background_path);
  free(background_path);

  char* news_path = malloc(strlen(path)+strlen("/assets/news.html")+1);
  strcpy(news_path, path);
  strcat(news_path, "/assets/news.html");
  strcpy(application.news.html, news_path);
  free(news_path);

  char* help_path = malloc(strlen(path)+strlen("/assets/help.html")+1);
  strcpy(help_path, path);
  strcat(help_path, "/assets/help.html");
  strcpy(application.help.html, help_path);
  free(help_path);

  free(path);
}

