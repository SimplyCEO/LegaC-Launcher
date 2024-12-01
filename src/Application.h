#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

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

GtkWidget*  _A_Box_Create(GtkWidget* container, GtkOrientation orientation);
void        _A_Box_Resize(GtkWidget* box, int size_x, int size_y);

LogDisplay* _A_Text_Logger(GtkWidget* widget);

GtkWidget*  _A_Window_Create(const char* window_name, int size_x, int size_y, GtkWindowPosition window_position);
void        _A_Window_Resize(GtkWidget* window, int size_x, int size_y);

void        _A_Initialise(int argc, char* argv[]);
void        _A_Loop(void);

static struct ApplicationClass
{ struct BoxWidget
  { GtkWidget* (*Create)(GtkWidget* container, GtkOrientation orientation);
    void       (*Resize)(GtkWidget* box, int size_x, int size_y);
  } Box;
  struct TextWidget
  { LogDisplay* (*Logger)(GtkWidget* widget);
  } Text;
  struct WindowWidget
  { GtkWidget* (*Create)(const char* window_name, int size_x, int size_y, GtkWindowPosition window_position);
    void       (*Resize)(GtkWidget* window, int size_x, int size_y);
  } Window;
  void (*Initialise)(int argc, char* argv[]);
  void (*Loop)(void);
}
CApplication =
{ { _A_Box_Create,    _A_Box_Resize    },
  { _A_Text_Logger                     },
  { _A_Window_Create, _A_Window_Resize },
    _A_Initialise,    _A_Loop
};

#endif

