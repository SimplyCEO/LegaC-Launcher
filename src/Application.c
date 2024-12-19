#include "data/Application.h"

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

GtkWidget*
_A_Box_Create(GtkWidget* container, GtkOrientation orientation)
{
  GtkWidget *box = gtk_box_new(orientation, 0);
  if (container != NULL)
  { gtk_container_add(GTK_CONTAINER(container), box); }

  return box;
}

void
_A_Box_Resize(GtkWidget* box, int size_x, int size_y)
{
  set_widget_size(box, size_x, size_y);
}

LogDisplay*
_A_Text_Logger(GtkWidget* widget)
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

unsigned char isMainWindowCreated = 0;
GtkWidget*
_A_Window_Create(const char* window_name, int size_x, int size_y, GtkWindowPosition window_position)
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

void
_A_Window_SetIcon(GtkWidget* window, const char* filepath)
{
  GdkPixbuf* icon = gdk_pixbuf_new_from_file(filepath, NULL);
  gtk_window_set_icon(GTK_WINDOW(window), icon);
  g_object_unref(icon);
}

void
_A_Window_SetBackground(GtkWidget* window, const char* filepath)
{
  GtkCssProvider* provider = gtk_css_provider_new();

  gchar* css = g_strdup_printf("window { background-image: url('%s'); background-size: 96px 96px; background-repeat: repeat; }", filepath);
  gtk_css_provider_load_from_data(provider, css, -1, NULL);

  GtkStyleContext* style_context = gtk_widget_get_style_context(window);
  gtk_style_context_add_provider(style_context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  g_free(css);
  g_object_unref(provider);
}

void
_A_Window_Resize(GtkWidget* window, int size_x, int size_y)
{
  set_widget_size(window, size_x, size_y);
}

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

