#include <WechatConfig.h>
#include <cJSON.h>
#include <StringexSha256WithRsa.h>

WechatConfig *WechatConfigCreate(MainConfig *mainConfig, WechatConfig **wcConfig, const char *section)
{
    if (mainConfig && mainConfig->injectObject)
    {
        if (ObjectCreate((void **)wcConfig, sizeof(WechatConfig)))
        {
            InjectCommand injectCommand[] = {
                INJECT_COMMAND_STRING("appid", offsetof(WechatConfig, appid)),
                INJECT_COMMAND_STRING("secret", offsetof(WechatConfig, secret)),
                INJECT_COMMAND_STRING("mchid", offsetof(WechatConfig, mchid)),
                INJECT_COMMAND_STRING("serial_no", offsetof(WechatConfig, serial_no)),
                INJECT_COMMAND_STRING("apiclient_key", offsetof(WechatConfig, apiclient_key)),
                INJECT_COMMAND_STRING("apiclient_cert", offsetof(WechatConfig, apiclient_cert)),
                INJECT_COMMAND_STRING("notify_url", offsetof(WechatConfig, notify_url)),
                INJECT_COMMAND_STRING("apiv3_key", offsetof(WechatConfig, apiv3_key)),
                INJECT_COMMAND_NULL};
            InjectCommandInit(mainConfig->injectObject, injectCommand, *wcConfig, "WECHAT_PAYMENT");
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
