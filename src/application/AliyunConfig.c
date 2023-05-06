#include <AliyunConfig.h>
#include <MainConfig.h>
#include <MemoryInject.h>

AliyunConfig *AliyunConfigCreate(MainConfig  *mainConfig, AliyunConfig **aliyunConfig, const char *section)
{
    if (mainConfig && mainConfig->injectObject)
    {
        if (ObjectCreate((void **)aliyunConfig, sizeof(AliyunConfig)))
        {
            InjectCommand injectCommand[] = {
                INJECT_COMMAND_STRING("AccessKeyId", offsetof(AliyunConfig, AccessKeyId)),
                INJECT_COMMAND_STRING("AccessKeySecret", offsetof(AliyunConfig, AccessKeySecret)),
                INJECT_COMMAND_STRING("ProductKey", offsetof(AliyunConfig, ProductKey)),
                INJECT_COMMAND_STRING("DeviceName", offsetof(AliyunConfig, DeviceName)),
                INJECT_COMMAND_STRING("Format", offsetof(AliyunConfig, Format)),
                INJECT_COMMAND_STRING("Version", offsetof(AliyunConfig, Version)),
                INJECT_COMMAND_STRING("SignatureMethod", offsetof(AliyunConfig, SignatureMethod)),
                INJECT_COMMAND_STRING("SignatureVersion", offsetof(AliyunConfig, SignatureVersion)),
                INJECT_COMMAND_STRING("RegionId", offsetof(AliyunConfig, RegionId)),
                INJECT_COMMAND_STRING("TopicFullName", offsetof(AliyunConfig, TopicFullName)),
                INJECT_COMMAND_NULL};
            InjectCommandInit(mainConfig->injectObject, injectCommand, *aliyunConfig, "ALIIOT");
            return (*aliyunConfig);
        }
    }
    return NULL;
}
void AliyunConfigDelete(AliyunConfig *aliyunConfig)
{
    ObjectDelete(aliyunConfig);
}