#include <wangyonglin/wangyonglin.h>
#include <https_post.h>
#include <SnowFlake.h>
#include <cJSON.h>

#include <string_by_hex.h>
#include <string_by_id.h>

#include <string_by_log.h>
#include <Encrypt/SHA256WithRSA.h>
#include <Encrypt/HMAC_SHA1.h>

#include <HTTPDServer.h>

#include <byte_by_this.h>

#include <WechatHttps.h>

#define MSG_QUE_KEY_ID 1996 // 消息队列标识

int main(int argc, char *argv[])
{
  config_t *config;
  config_create(&config, argc, argv);
  SnowFlakeInit(1, 1, 10);
  HTTPDServerStart(config);
  return 0;
}
