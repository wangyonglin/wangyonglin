#include <AliyunConfig.h>
#include <string_by_inject.h>
#include <wangyonglin/wangyonglin.h>

AliyunConfig *AliyunConfigCreate(Config_t  *config, AliyunConfig **aliConfig, const char *section)
{
    if (config && config->inject)
    {
        if (object_create((void **)aliConfig, sizeof(AliyunConfig)))
        {
            inject_command_t commands[] = {
                inject_string_command("AccessKeyId", offsetof(AliyunConfig, AccessKeyId)),
                inject_string_command("AccessKeySecret", offsetof(AliyunConfig, AccessKeySecret)),
                inject_string_command("ProductKey", offsetof(AliyunConfig, ProductKey)),
                inject_string_command("DeviceName", offsetof(AliyunConfig, DeviceName)),
                inject_string_command("Format", offsetof(AliyunConfig, Format)),
                inject_string_command("Version", offsetof(AliyunConfig, Version)),
                inject_string_command("SignatureMethod", offsetof(AliyunConfig, SignatureMethod)),
                inject_string_command("SignatureVersion", offsetof(AliyunConfig, SignatureVersion)),
                inject_string_command("RegionId", offsetof(AliyunConfig, RegionId)),
                inject_string_command("TopicFullName", offsetof(AliyunConfig, TopicFullName)),
                inject_null_command};
            inject_build(config->inject, commands, *aliConfig, "ALIIOT");
            return (*aliConfig);
        }
    }
    return NULL;
}
void AliyunConfigDelete(AliyunConfig *aliConfig)
{
    object_delete(aliConfig);
}