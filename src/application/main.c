#include <wangyonglin/wangyonglin.h>
#include <SnowWorkerM1.h>
#include <SnowFlake.h>
#include <Encrypt/SHA256WithRSA.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wangyonglin/file.h>
#include <application/httpd.h>
#include <application/aliapis.h>
#include <application/aliutils.h>
#include <Encrypt/HMAC_SHA1.h>
#include <Encrypt/Base64.h>
#include <wangyonglin/list.h>
#include <wangyonglin/conf.h>
#include <https/client.h>
#include <wangyonglin/string.h>
#include <Encrypt/URL.h>
#include <wangyonglin/list.h>
#include <application/localapis.h>
#include <wangyonglin/object.h>
#include <wangyonglin/buffer.h>
#include <application/https.h>
#define MSG_QUE_KEY_ID 1996 // 消息队列标识

int main(int argc, char *argv[])
{
  https_callback *callback;
  app_t *app;
  application_create(&app, argc, argv);

  SnowFlakeInit(1, 1, 10);
  logerr(app->log, app->options->confname.outstring);
  localapis_t *aliapis;
  localapis_init(&aliapis, app->options->confname, "ALIIOT");
  HttpsPublish(aliapis, "866714043075174", "wangyonglin", 11);
  localapis_free(aliapis);
  return 0;
}
