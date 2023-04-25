#include <wangyonglin/wangyonglin.h>
#include <https_post.h>

#include <cJSON.h>

#include <string_by_hex.h>
#include <string_by_id.h>

#include <string_by_log.h>
#include <SHA256WithRSA.h>
#include <HMAC_SHA1.h>

#include <HTTPDServer.h>

#include <WechatHttps.h>
#include <aes_256_gcm.h>
#include <base64.h>
#include <zlog/zlog.h>
#define MSG_QUE_KEY_ID 1996 // 消息队列标识

int main(int argc, char *argv[])
{

  Config_t *config = ConfigInit(argc, argv);

  HTTPDServerStart(config);

  return 0;
}
