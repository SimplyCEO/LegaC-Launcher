#ifndef APPLICATION_DATA_HEADER
#define APPLICATION_DATA_HEADER

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static struct ApplicationDefinition
{ char* name;
  char* version;
} application = { "LegaC Launcher", "v0.0.3" };

typedef struct
{ GtkWidget *text_view;
  GAsyncQueue *queue;
  guint source_id;
} LogDisplay;

#endif

