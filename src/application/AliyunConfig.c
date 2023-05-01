#include <AliyunConfig.h>
#include <MainConfig.h>
#include <MemoryInject.h>

AliyunConfig *AliyunConfigCreate(MainConfig  *mainConfig, AliyunConfig **aliyunConfig, const char *section)
{
    if (mainConfig && mainConfig->memoryInject)
    {
        if (ObjectCreate((void **)aliyunConfig, sizeof(AliyunConfig)))
        {
            MemoryInject_Command commands[] = {
                MEMORYINJECT_STRING_COMMAND("AccessKeyId", offsetof(AliyunConfig, AccessKeyId)),
                MEMORYINJECT_STRING_COMMAND("AccessKeySecret", offsetof(AliyunConfig, AccessKeySecret)),
                MEMORYINJECT_STRING_COMMAND("ProductKey", offsetof(AliyunConfig, ProductKey)),
                MEMORYINJECT_STRING_COMMAND("DeviceName", offsetof(AliyunConfig, DeviceName)),
                MEMORYINJECT_STRING_COMMAND("Format", offsetof(AliyunConfig, Format)),
                MEMORYINJECT_STRING_COMMAND("Version", offsetof(AliyunConfig, Version)),
                MEMORYINJECT_STRING_COMMAND("SignatureMethod", offsetof(AliyunConfig, SignatureMethod)),
                MEMORYINJECT_STRING_COMMAND("SignatureVersion", offsetof(AliyunConfig, SignatureVersion)),
                MEMORYINJECT_STRING_COMMAND("RegionId", offsetof(AliyunConfig, RegionId)),
                MEMORYINJECT_STRING_COMMAND("TopicFullName", offsetof(AliyunConfig, TopicFullName)),
                MEMORYINJECT_NULL_COMMAND};
            MemoryInjectInster(mainConfig->memoryInject, commands, *aliyunConfig, "ALIIOT");
            return (*aliyunConfig);
        }
    }
    return NULL;
}
void AliyunConfigDelete(AliyunConfig *aliyunConfig)
{
    ObjectDelete(aliyunConfig);
}