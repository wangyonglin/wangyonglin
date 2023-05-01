#include <WechatConfig.h>
#include <string_by_this.h>
#include <cJSON.h>
#include <SHA256WithRSA.h>

#include <string_by_hex.h>
#include <string_by_id.h>

#include <string_by_log.h>

#include <string_by_timestamp.h>
#include <string_by_inject.h>
#include <StringexSha256WithRsa.h>

WechatConfig *WechatConfigCreate(MainConfig *mainConfig, WechatConfig **wcConfig, const char *section)
{
    if (mainConfig && mainConfig->memoryInject)
    {
        if (ObjectCreate((void **)wcConfig, sizeof(WechatConfig)))
        {
            MemoryInject_Command commands[] = {
                MEMORYINJECT_STRING_COMMAND("appid", offsetof(WechatConfig, appid)),
                MEMORYINJECT_STRING_COMMAND("secret", offsetof(WechatConfig, secret)),
                MEMORYINJECT_STRING_COMMAND("mchid", offsetof(WechatConfig, mchid)),
                MEMORYINJECT_STRING_COMMAND("serial_no", offsetof(WechatConfig, serial_no)),
                MEMORYINJECT_STRING_COMMAND("apiclient_key", offsetof(WechatConfig, apiclient_key)),
                MEMORYINJECT_STRING_COMMAND("apiclient_cert", offsetof(WechatConfig, apiclient_cert)),
                MEMORYINJECT_STRING_COMMAND("notify_url", offsetof(WechatConfig, notify_url)),
                MEMORYINJECT_STRING_COMMAND("apiv3_key", offsetof(WechatConfig, apiv3_key)),
                MEMORYINJECT_NULL_COMMAND};
            MemoryInjectInster(mainConfig->memoryInject, commands, *wcConfig, "WECHAT_PAYMENT");
            return (*wcConfig);
        }
    }
    return NULL;
}

void WechatConfigDelete(WechatConfig *wcConfig)
{
    StringexDelete(wcConfig->appid);
    StringexDelete(wcConfig->secret);
    StringexDelete(wcConfig->mchid);
    StringexDelete(wcConfig->serial_no);
    StringexDelete(wcConfig->apiclient_key);
    StringexDelete(wcConfig->notify_url);
    ObjectDelete(wcConfig);
}
