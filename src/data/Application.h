#ifndef APPLICATION_DATA_HEADER
#define APPLICATION_DATA_HEADER

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

typedef struct
{ char* html;
  char* url;
} Page;

static struct ApplicationDefinition
{ char* name;
  char* icon;
  char* background;
  char* version;
  Page  news;
  Page  help;
} application =
{
  "LegaC Launcher", "./assets/icon.png", "./assets/background.png", "v0.0.4",
  { "./assets/news.html", "https://mcupdate.tumblr.com" },
  { "./assets/help.html", "https://gitlab.com/SimplyCEO/LegaC-Launcher/-/issues" }
};

typedef struct
{ GtkWidget *text_view;
  GAsyncQueue *queue;
  guint source_id;
} LogDisplay;

#endif

