#ifndef INTERNET_HEADER
#define INTERNET_HEADER

unsigned char _I_HandleData(const char* filepath, const char* url);

static struct InternetClass
{
  unsigned char (*HandleData)(const char* filepath, const char* url);
}
CInternet =
{
  _I_HandleData
};

#endif

