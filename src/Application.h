#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "data/Application.h"

void        _A_Background_Window(GtkWidget* window, const char* filepath);

GtkWidget*  _A_Create_Box(GtkWidget* container, GtkOrientation orientation);
GtkWidget*  _A_Create_Entry(const char* description, GtkWidget* container, const char* position);
GtkWidget*  _A_Create_Label(const char* title, GtkWidget* container, const char* position);
GtkWidget*  _A_Create_ProgressBar(GtkWidget* container, const char* position);
GtkWidget*  _A_Create_Window(const char* window_name, int size_x, int size_y, GtkWindowPosition window_position);

void        _A_Event_Align(GtkWidget* widget, const char* position, GtkAlign align);
void        _A_Event_Attach(GtkWidget* widget, GtkWidget* container, const char* position, gboolean h_expand, gboolean v_expand, unsigned short padding);
void        _A_Event_Connect(GtkWidget* widget, const char* mode, void (*function)(GtkWidget*,gpointer), gpointer data);
void        _A_Event_Resize(GtkWidget* box, int size_x, int size_y);

void        _A_Icon_Window(GtkWidget* window, const char* filepath);

void        _A_Update_Entry_Description(GtkWidget* widget, const char* description);
void        _A_Update_Entry_Text(GtkWidget* widget, const char* definition);
void        _A_Update_Label_Text(GtkWidget* widget, const char* title);
LogDisplay* _A_Update_Logger(GtkWidget* widget);

void        _A_Initialise(int argc, char* argv[]);
void        _A_Loop(void);
void        _A_SetupPath(void);

static struct ApplicationClass
{ struct WidgetBackground
  { void        (*Window)(GtkWidget* window, const char* filepath);
  } Background;
  struct WidgetCreate
  { GtkWidget*  (*Box)(GtkWidget* container, GtkOrientation orientation);
    GtkWidget*  (*Entry)(const char* description, GtkWidget* container, const char* position);
    GtkWidget*  (*Label)(const char* title, GtkWidget* container, const char* position);
    GtkWidget*  (*ProgressBar)(GtkWidget* container, const char* position);
    GtkWidget*  (*Window)(const char* window_name, int size_x, int size_y, GtkWindowPosition window_position);
  } Create;
  struct WidgetEvent
  { void        (*Align)(GtkWidget* widget, const char* position, GtkAlign align);
    void        (*Attach)(GtkWidget* widget, GtkWidget* container, const char* position, gboolean h_expand, gboolean v_expand, unsigned short padding);
    void        (*Connect)(GtkWidget* widget, const char* mode, void (*function)(GtkWidget*,gpointer), gpointer data);
    void        (*Resize)(GtkWidget* widget, int size_x, int size_y);
  } Event;
  struct WidgetIcon
  { void        (*Window)(GtkWidget* window, const char* filepath);
  } Icon;
  struct WidgetUpdate
  { struct EntryUpdate
    { void      (*Description)(GtkWidget* widget, const char* description);
      void      (*Text)(GtkWidget* widget, const char* definition);
    } Entry;
    struct LabelUpdate
    { void      (*Text)(GtkWidget* widget, const char* title);
    } Label;
    LogDisplay* (*Logger)(GtkWidget* widget);
  } Update;
  void (*Initialise)(int argc, char* argv[]);
  void (*Loop)(void);
  void (*SetupPath)(void);
}
CApplication =
{
  {
    _A_Background_Window
  },
  {
    _A_Create_Box,
    _A_Create_Entry,
    _A_Create_Label,
    _A_Create_ProgressBar,
    _A_Create_Window
  },
  {
    _A_Event_Align,
    _A_Event_Attach,
    _A_Event_Connect,
    _A_Event_Resize
  },
  {
    _A_Icon_Window
  },
  {
    {
      _A_Update_Entry_Description,
      _A_Update_Entry_Text
    },
    {
      _A_Update_Label_Text
    },
    _A_Update_Logger
  },
  _A_Initialise,
  _A_Loop,
  _A_SetupPath
};

#endif

