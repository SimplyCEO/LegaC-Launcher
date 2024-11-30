#ifndef FILE_MANAGER_HEADER
#define FILE_MANAGER_HEADER

int           _FM_Validate(const char* filepath);
int           _FM_OpenFile(const char* filepath, const char* mode);
int           _FM_Read(int fd, const char* buf, int len);
int           _FM_Write(int fd, const char* buf, int len);
unsigned char _FM_Seek(int fd, int offset, int whence);
unsigned char _FM_ReadLine(int fd, char* buf, int len);
int           _FM_CloseFile(int fd);
int           _FM_GetError(int fd);
FILE*         _FM_Stream(int fd);
void          _FM_Copy(int fd1, int fd2);
void          _FM_Flush(int fd);

static struct FileManagerClass
{
  int           (*Validate)(const char* filepath);
  int           (*OpenFile)(const char* filepath, const char* mode);
  int           (*Read)(int fd, const char* buf, int len);
  int           (*Write)(int fd, const char* buf, int len);
  unsigned char (*Seek)(int fd, int offset, int whence);
  unsigned char (*ReadLine)(int fd, char* buf, int len);
  int           (*CloseFile)(int fd);
  int           (*GetError)(int fd);
  FILE*         (*Stream)(int fd);
  void          (*Copy)(int fd1, int fd2);
  void          (*Flush)(int fd);
}
CFileManager =
{
  _FM_Validate,   _FM_OpenFile, _FM_Read,
  _FM_Write,      _FM_Seek,     _FM_ReadLine,
  _FM_CloseFile,  _FM_GetError, _FM_Stream,
  _FM_Copy,       _FM_Flush
};

#endif

