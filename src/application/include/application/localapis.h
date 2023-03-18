#if !defined(INCLUDE_APPLICATION_LOCALAPIS_H)
#define INCLUDE_APPLICATION_LOCALAPIS_H
#include <wangyonglin/package.h>
#include <wangyonglin/object.h>
typedef struct _localapis_t
{
    char *DeviceName;
    char *AccessKeySecret;
    char *Action;
    char *ProductKey;
    char *TopicFullName;
    char *Format;
    char *Version;
    char *AccessKeyId;
    char *Signature;
    char *SignatureMethod;
    char *SignatureVersion;
    char *RegionId;
    long maxInflight;
} localapis_t;

void localapis_init(localapis_t **aliapis, datasheet cfg, const char *section);
  void localapis_free(localapis_t *aliapis);
#endif