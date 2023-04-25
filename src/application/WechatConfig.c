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

WechatConfig *WechatConfigCreate(Config_t *config, WechatConfig **wcConfig, const char *section)
{
    if (config && config->inject)
    {
        if (object_create((void **)wcConfig, sizeof(WechatConfig)))
        {
            inject_command_t commands[] = {
                inject_string_command("appid", offsetof(WechatConfig, appid)),
                inject_string_command("secret", offsetof(WechatConfig, secret)),
                inject_string_command("mchid", offsetof(WechatConfig, mchid)),
                inject_string_command("serial_no", offsetof(WechatConfig, serial_no)),
                inject_string_command("apiclient_key", offsetof(WechatConfig, apiclient_key)),
                inject_string_command("apiclient_cert", offsetof(WechatConfig, apiclient_cert)),
                inject_string_command("notify_url", offsetof(WechatConfig, notify_url)),
                inject_string_command("apiv3_key", offsetof(WechatConfig, apiv3_key)),
                inject_null_command};
            inject_build(config->inject, commands, *wcConfig, "WECHAT_PAYMENT");
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
    object_delete(wcConfig);
}
