#if !defined(INCLUDE_APPLICATION_ALIYUNCONFIG_H)
#define INCLUDE_APPLICATION_ALIYUNCONFIG_H

#include <MainConfig.h>

#include <Stringex.h>
typedef struct _AliyunConfig_t
{
    Stringex DeviceName;
    Stringex AccessKeySecret;
    Stringex Action;
    Stringex ProductKey;
    Stringex TopicFullName;
    Stringex Format;
    Stringex Version;
    Stringex AccessKeyId;
    Stringex Signature;
    Stringex SignatureMethod;
    Stringex SignatureVersion;
    Stringex RegionId;
    long maxInflight;
} AliyunConfig;

AliyunConfig *AliyunConfigCreate(MainConfig  *mainConfig, AliyunConfig **apis, const char *section);
void AliyunConfigDelete(AliyunConfig *aliConfig);
#endif