#include <application/https.h>
#include <curl/curl.h>
void https_get(char *urlsting)
{
  CURL *curl;

CURLcode res;

curl_global_init(CURL_GLOBAL_DEFAULT);

curl = curl_easy_init();

if(curl) {

curl_easy_setopt(curl, CURLOPT_URL, "https://www.baidu.com/");

/* Perform the request, res will get the return code */

res = curl_easy_perform(curl);

/* Check for errors */

if(res != CURLE_OK)

fprintf(stderr, "curl_easy_perform() failed: %s\n",

curl_easy_strerror(res));

/* always cleanup */

curl_easy_cleanup(curl);

}

curl_global_cleanup();
}