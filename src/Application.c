#include <gtk/gtk.h>

/* Quit the application after main window destroy. */
void
on_window_destroy(GtkWidget *widget, gpointer data)
{ gtk_main_quit(); }

/* Resize the widget with X/Y scale. */
void
set_widget_size(GtkWidget* widget, int size_x, int size_y)
{ gtk_widget_set_size_request(widget, size_x, size_y); }

/* CApplication.Box.Create:
 * Create a box widget, child of a container, with the desired orientation. */
GtkWidget*
_A_Box_Create(GtkWidget* container, GtkOrientation orientation)
{
  GtkWidget *box = gtk_box_new(orientation, 0);
  if (container != NULL)
  { gtk_container_add(GTK_CONTAINER(container), box); }

  return box;
}

/* CApplication.Box.Resize
 * Resize a box widget with given position. */
void
_A_Box_Resize(GtkWidget* box, int size_x, int size_y)
{
  set_widget_size(box, size_x, size_y);
}

/* CApplication.Window.Create:
 * Create window widget based on name, size, and position. */
GtkWidget*
_A_Window_Create(const char* window_name, int size_x, int size_y, GtkWindowPosition window_position)
{
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_title(GTK_WINDOW(window), window_name);
  gtk_window_set_default_size(GTK_WINDOW(window), size_x, size_y);
  gtk_window_set_position(GTK_WINDOW(window), window_position);
  gtk_container_set_border_width(GTK_CONTAINER(window), 8);

  /* Destroy window after quitting application. */  
  g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

  return window;
}

/* CApplication.Window.Resize
 * Resize a box widget with given position. */
void
_A_Window_Resize(GtkWidget* window, int size_x, int size_y)
{
  set_widget_size(window, size_x, size_y);
}

/* CApplication.Initialise:
 * Start GTK application. */
void
_A_Initialise(int argc, char* argv[])
{
  gtk_init(&argc, &argv);
}

/* CApplication.Loop:
 * Avoid closing the application while main window is still running. */
void
_A_Loop(void)
{
  gtk_main();
}

