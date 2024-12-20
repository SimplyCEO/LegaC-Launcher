Documentation
=============

Most of the sections are explained.

Developers are encouraged to use this as a track/tutorial page when developing the launcher.

Data
====

<details><summary>Application</summary>

```c
typedef struct
{
  char* html;
  char* url;
} Page;

static struct ApplicationDefinition
{
  char* name;
  char* icon;
  char* background;
  char* version;
  Page  news;
  Page  help;
} application;

typedef struct
{
  GtkWidget *text_view;
  GAsyncQueue *queue;
  guint source_id;
} LogDisplay;
```

</details>

<details><summary>Downloader</summary>

```c
typedef enum
{
  LWJGL = 0,
  AUTHLIB,
  APACHE_COMMONS_IO,
  APACHE_COMMONS_LANG3,
  GSON,
  GUAVA,
  ICU4J,
  JOPT_SIMPLE,
  LOG4J_CORE,
  LOG4J_API,
  PAULSCODE_CODECJORBIS,
  PAULSCODE_CODECWAV,
  PAULSCODE_JAVA_SOUND,
  PAULSCODE_LWJGL_OPENAL,
  PAULSCODE_SOUND_SYSTEM,
  NETTY_AIO,
  LIBRARY_LAST
} Library;
```

</details>

<details><summary>FileManager</summary>

```c
#define MAX_FM_FILES 10
static struct ArchivedFiles
{ char path[256];
  unsigned char txt;
  FILE* stream;
} file[MAX_FM_FILES];
```

</details>

<details><summary>settings</summary>

```c
typedef struct
{
  struct MCVer
  {
    unsigned char major;
    unsigned char minor;
    unsigned char patch;
  } version;
  char username[16];
  struct MCDir
  {
    char game[128];
    char asset[128];
  } directory;
} MinecraftSettings;
extern MinecraftSettings mc;
```

</details>

<details><summary>game</summary>

```c
char* progress_label_text = "Preparing to download...";
GtkWidget* progress_label;

gint dl_total = 0;
gint dl_current = 0;
```

</details>

<details><summary>launcher</summary>

```c
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
```

</details>

<br>

Function (class)
================

<details><summary>CApplication</summary>

Private
-------

- `on_window_destroy(GtkWidget* widget, gpointer data)`:<br>
  Quit the application after main window destroy.<br>
  Return type: `void`.

- `set_widget_size(GtkWidget* widget, int size_x, int size_y)`:<br>
  Resize the <b>widget</b> using the provided <b>size</b>.<br>
  Return type: `void`.

- `process_log_messages(LogDisplay *log_display)`:<br>
  Process the log messages in the queue.<br>
  Return type: `gboolean`.

Public
------

- `Box`:<br>
  * `Create(GtkWidget* container, GtkOrientation orientation)`:<br>
    Create a box <b>widget</b> attached to a <b>container</b> with a <b>[GTK orientation](https://docs.gtk.org/gtk3/enum.Orientation.html#members)</b>.<br>
    If a `NULL` is defined for <b>container</b>, it will not attach.<br>
    Return type: `GtkWidget*`.<br>
  * `Resize(GtkWidget* box, int size_x, int size_y)`:<br>
    Same as <b>set_widget_size</b>.<br>
    Return type: `void`.

- `Text`:<br>
  * `Logger(GtkWidget* widget)`:<br>
    Create a read-only log box.<br>
    Return type: `LogDisplay*`.

- `Window`:<br>
  * `Create(const char* window_name, int size_x, int size_y, GtkWindowPosition window_position)`:<br>
    Create window <b>widget</b> based on <b>name</b>, <b>size</b>, and <b>[GTK window position](https://docs.gtk.org/gtk3/enum.WindowPosition.html#members)</b>.<br>
    First window will be treated as main window.<br>
    Return type: `GtkWidget*`.<br>
  * `Resize(GtkWidget* box, int size_x, int size_y)`:<br>
    Same as <b>set_widget_size</b>.<br>
    Return type: `void`.<br>
  * `SetIcon(GtkWidget* window, const char* filepath)`:<br>
    Set the application icon, from a <b>file path</b>.<br>
    Return type: `void`.<br>
  * `SetBackground(GtkWidget* window, const char* filepath)`:<br>
    Set the background image, from a <b>file path</b>, forcing the size to `96x96`.<br>
    Return type: `void`.

- `Initialise(int argc, char** argv)`:<br>
  Start GTK application.<br>
  Return type: `void`.

- `Loop(void)`:<br>
  Avoid closing the application while main window is still running.<br>
  Return type: `void`.

</details>

<details><summary>CDownloader</summary>

Private
-------

- `extract_response_url(const char *data)`:<br>
  Get the <b>hash</b> URL from response data.<br>
  Return type: `char*`.

Public
------

- `FindGame(const char* version, const char* type, int force)`:<br>
  Get Minecraft <b>client</b> from server.<br>
  Return type: `void`.

- `FindLibrary(Library library, const char* version)`:<br>
  Get Minecraft libraries from server, based on the game version.<br>
  Return type: `void`.

</details>

<details><summary>CInternet</summary>

Private
-------

- `write_data(void* contents, int size, int nmemb, FILE* file)`:<br>
  Handler for <b>CURL_WRITEDATA</b> to write received data to a file.<br>
  Return type: `int`.


Public
------

- `HandleData(const char* filepath, const char* url)`:<br>
  Output received data to a <b>file</b>.<br>
  Return type: `unsigned char`.

</details>

<details><summary>CFileManager</summary>

Private
-------

They are same variants of `stdio.h`, but cross-platform.

Public
------

- `Validate(const char* filepath)`:<br>
  Check if the file exists.<br>
  Return type: `int`.

- `OpenFile(const char* filepath, const char* mode)`:<br>
  Open file to a <b>file array</b>.<br>
  Return type: `int`.

- `Read(int fd, const char* buf, int len)`:<br>
  `_fread((void*)buf, 1, len, fd);`<br>
  Return type: `int`.

- `Write(int fd, const char* buf, int len)`:<br>
  `_fwrite((void*)buf, 1, len, fd);`<br>
  Return type: `int`.

- `Seek(int fd, int offset, int whence)`:<br>
  `!!_fseek(fd, offset, whence);`<br>
  Return type: `unsigned char`.

- `ReadLine(int fd, char* buf, int len)`:<br>
  `_fgets(buf, len, fd) != NULL;`<br>
  Return type: `unsigned char`.

- `CloseFile(int fd)`:<br>
  Close file from <b>file array</b>.<br>
  Return type: `int`.

- `GetError(int fd)`:<br>
  Check if the <b>end</b> of the file has reached.<br>
  Return type: `int`.

- `Stream(int fd)`:<br>
  Return the <b>file stream</b> of <b>file array</b>.<br>
  Return type: `FILE*`.

- `Copy(int fd1, int fd2)`:<br>
  Copy contents of `fd1` to `fd2`.<br>
  Return type: `void`.

- `CopyFromPath(const char* filepath1, const char* filepath2)`:<br>
  Same as `Copy`, but instead uses a <b>file path</b>.<br>
  Return type: `void`.

- `Flush(int fd)`:<br>
  Empty file position in <b>file array</b> and remove it from system.<br>
  Return type: `void`.

- `ReadFile(int fd)`:<br>
  Return file content from file positioned in <b>file array</b>.
  Return type: `char*`.

- `ReadFileFromPath(const char* filepath)`:<br>
  Same as `ReadFile`, but instead uses a <b>file path</b>.<br>
  Return type: `char*`.

</details>

<br>

Function
========

<details><summary>args</summary>

Private
-------

- `help(void)`:<br>
  Output useful <b>information</b> about `LegaC`.<br>
  Return type: `void`.

Public
------

- `parse_arguments(int argc, char* argv[])`:<br>
  Handle flags and/or arguments into intern events.<br>
  Return type: `unsigned char`.

- `download_libraries(Library library, const char* version)`:<br>
  Download game <b>libraries based on</b> provided <b>version</b>.<br>
  Return type: `unsigned char`.

- `download_game(const char* version, unsigned char from_gtk)`:<br>
  Download game <b>client based on</b> provided <b>version</b>.<br>
  Return type: `unsigned char`.

</details>

<details><summary>game</summary>

Private
-------

- `play_game(const char* mc_version, const char* mc_username, const char* mc_gamedir, const char* mc_class)`:<br>
  Build the <b>JavaVM script</b> to launch the game.<br>
  Return type: `void`.

- `log_message(LogDisplay *logger, const gchar *message)`:<br>
  Write text to `logger` display.<br>
  Return type: `void`.

- `update_progress_bar(gpointer progress_bar)`:<br>
  Update `progress_bar` widget.<br>
  Return type: `void`.

- `download_library(Library library)`:<br>
  Download the pointed <b>library</b> `Library`.<br>
  Return type: `unsigned char`.

Public
------

- `gameLogWindow(gpointer data)`:<br>
  Handle log window to `logger`.<br>
  Return type: `void`.

- `initialise_game(gpointer data)`:<br>
  Launch game after pressing the <b>Play</b> button.
  Return type: `void`.

</details>

<details><summary>launcher</summary>

Private
-------

- `on_version_choose(GtkWidget* widget, gpointer data)`:<br>
  Assign game version to <b>intern game data</b>.<br>
  Return type: `void`.

- `on_click_open_website(GtkWidget* widget, gpointer data)`:<br>
  Open website URL <b>after clicking a button</b>.<br>
  Return type: `void`.

- `on_username_changed(GtkWidget* widget, gpointer data)`:<br>
  Assign user name to <b>intern game data</b>.<br>
  Return type: `void`.

- `onLogWindow(GtkWidget* widget, LogDisplay* log_display)`:<br>
  Handle log window on a separate thread.<br>
  Return type: `void`.

- `on_play_clicked(GtkWidget* widget, LogDisplay* log_display)`:<br>
  Handle <b>Play</b> button <b>click event</b> on a separate thread.<br>
  Return type: `void`.

- `InitialiseWindow(const char* window_name, const char* window_icon, const char* window_background, int size_x, int size_y, GtkWindowPosition window_position)`:<br>
  Create launcher window.<br>
  Return type: `void`.

- `CreateHeader(void)`:<br>
  Create launcher header.<br>
  Return type: `void`.

- `CreateDocker(void)`:<br>
  Create launcher docker.<br>
  Return type: `void`.

Public
------

- `startApplication(int argc, char** argv)`:<br>
  Handle application start based on console arguments.<br>
  Return type: `void`.

</details>

