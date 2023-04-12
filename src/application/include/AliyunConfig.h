#if !defined(INCLUDE_APPLICATION_ALIYUNCONFIG_H)
#define INCLUDE_APPLICATION_ALIYUNCONFIG_H

#include <wangyonglin/package.h>
#include <wangyonglin/wangyonglin.h>
#include <string_by_this.h>
typedef struct _AliyunConfig_t
{
    string_by_t DeviceName;
    string_by_t AccessKeySecret;
    string_by_t Action;
    string_by_t ProductKey;
    string_by_t TopicFullName;
    string_by_t Format;
    string_by_t Version;
    string_by_t AccessKeyId;
    string_by_t Signature;
    string_by_t SignatureMethod;
    string_by_t SignatureVersion;
    string_by_t RegionId;
    long maxInflight;
} AliyunConfig;

AliyunConfig *AliyunConfigCreate(Config_t  *config, AliyunConfig **apis, const char *section);
void AliyunConfigDelete(AliyunConfig *aliConfig);
#endif