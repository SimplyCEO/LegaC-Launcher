#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#include "FileManager.h"

static int
write_data(void* contents, int size, int nmemb, FILE* file)
{
  if (file == NULL)
  { fprintf(stderr, "No valid file given.\n"); return 1; }
  return fwrite(contents, size, nmemb, file);
}

unsigned char
_I_HandleData(const char* filepath, const char* url)
{
  if ((filepath == NULL) | (url == NULL))
  { fprintf(stderr, "No FILE/URL given! Expected a valid FILE/URL.\n"); return 1; }

  CURL* curl;
  CURLcode response = 0;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if (curl)
  { int file = CFileManager.OpenFile(filepath, "wb");
    if (file == -1)
    { fprintf(stderr, "Could not write to %s.\n", filepath); return 1; }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, CFileManager.Stream(file));
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);

    response = curl_easy_perform(curl);
    if (response != CURLE_OK)
    { fprintf(stderr, "cURL error: %s.\n", curl_easy_strerror(response)); }

    CFileManager.CloseFile(file);
    curl_easy_cleanup(curl);
  }

  return 0;
}

