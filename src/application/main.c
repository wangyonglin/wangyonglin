#include <wangyonglin/wangyonglin.h>
#include <SnowWorkerM1.h>
#include <SnowFlake.h>
#include <SHA256WithRSA.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wangyonglin/file.h>
#include <application/httpd.h>
#include <application/aliapis.h>
#include <application/aliutils.h>
#include <HMAC_SHA1.h>
#include <Base64.h>
#include <wangyonglin/list.h>
#include <wangyonglin/conf.h>
#include <https/client.h>
#include <wangyonglin/string.h>

#define MSG_QUE_KEY_ID 1996 // 消息队列标识

int main(int argc, char *argv[])
{
  app_t *app;
  ok_t ret = -1;
  application_create(&app, argc, argv);
  SnowFlakeInit(1, 1, 10);
  httpd_create(app);
  httpd_start();
  httpd_delete();
  return ret;
}
