#ifndef APPLICATION_DATA_HEADER
#define APPLICATION_DATA_HEADER

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

typedef struct
{ int x;
  int y;
} ApplicationWindowSize;

typedef struct
{ unsigned short xms;
  unsigned short xmx;
  char extra_arguments[2048];
  ApplicationWindowSize size;
} ApplicationSettings;

typedef struct
{ char html[128];
  char* url;
} Page;

typedef struct
{ char path[1024];
  char* name;
  char icon[128];
  char background[128];
  char* version;
  ApplicationSettings settings;
  Page  news;
  Page  help;
} ApplicationData;
extern ApplicationData application;

typedef struct
{ GtkWidget *text_view;
  GAsyncQueue *queue;
  guint source_id;
} LogDisplay;

#endif

