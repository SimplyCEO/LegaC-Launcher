#ifndef APPLICATION_DATA_HEADER
#define APPLICATION_DATA_HEADER

#include <gtk/gtk.h>

static struct ApplicationDefinition
{ char* name;
  char* version;
} application = { "LegaC Launcher", "v0.0.2" };

typedef struct
{ GtkWidget *text_view;
  GAsyncQueue *queue;
  guint source_id;
} LogDisplay;

#endif

