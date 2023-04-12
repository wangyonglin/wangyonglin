#include <wangyonglin/wangyonglin.h>
#include <https_post.h>
#include <SnowFlake.h>
#include <cJSON.h>

#include <string_by_hex.h>
#include <string_by_id.h>

#include <string_by_log.h>
#include <SHA256WithRSA.h>
#include <HMAC_SHA1.h>

#include <HTTPDServer.h>

#include <byte_by_this.h>

#include <WechatHttps.h>
#include <aes_256_gcm.h>
#include <base64.h>

#define MSG_QUE_KEY_ID 1996 // 消息队列标识

int main(int argc, char *argv[])
{
  Config_t *config = ConfigInit(argc, argv);
  logerr(config->log, "eee");
  SnowFlakeInit(1, 1, 10);
  HTTPDServerStart(config);

  return 0;
}
