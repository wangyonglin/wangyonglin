#include <aliyun_iot_api.h>
#include <SnowFlake.h>


ok_t AliPubParamsCreate(AliPubParams_t **AliPubParams, config_t *config)
{
    // if (config_verify(config) != Ok)
    // {
    //     return ErrorException;
    // }

    // if (!malloc_create((void **)AliPubParams, sizeof(AliPubParams_t)))
    // {
    //     return NullPointerException;
    // }

    // int aliyun_iot_api_commands_size = sizeof(aliyun_iot_api_commands) / sizeof(aliyun_iot_api_commands[0]);
    // if (conf_mapping(config->conf, (void **)AliPubParams, sizeof(AliPubParams_t), "ALIIOT_API_PUBLIC", aliyun_iot_api_commands, aliyun_iot_api_commands_size) != Ok)
    // {
    //     logerr(config->logger, "https_server_initializing conf_mapping   failed");
    //     return ErrorException;
    // }
    return Ok;
}

ok_t AliPubParamsInit(AliPubParams_t *AliPubParams, config_t *config)
{
    if (!AliPubParams)
    {
        return NullPointerException;
    }

    char SignatureNonce[64] = {0};
    sprintf(SignatureNonce, "%ld", SnowFlake_IdGenerator());
    string_create(config->allocate, (unsigned char **)&AliPubParams->SignatureNonce, SignatureNonce, strlen(SignatureNonce));
    cJSON *root = cJSON_CreateObject();
    char *outroot;
    if (root)
    {
        cJSON_AddStringToObject(root, "Format", AliPubParams->Format);
        cJSON_AddStringToObject(root, "Version", AliPubParams->Version);
        cJSON_AddStringToObject(root, "AccessKeyId", AliPubParams->AccessKeyId);
        cJSON_AddStringToObject(root, "SignatureMethod", AliPubParams->SignatureMethod);
        cJSON_AddStringToObject(root, "SignatureVersion", AliPubParams->SignatureVersion);
        cJSON_AddStringToObject(root, "RegionId", AliPubParams->RegionId);
        cJSON_AddStringToObject(root, "Timestamp", AliPubParams->Timestamp);
        cJSON_AddStringToObject(root, "SignatureNonce", AliPubParams->SignatureNonce);
        outroot = cJSON_PrintUnformatted(root);
        printf(outroot);
        free(outroot);
        cJSON_Delete(root);
    }
}
void AliPubParamsDelete(AliPubParams_t *AliPubParams)
{
   // deallocate(AliPubParams);
}

ok_t AliPubParams_toString(config_t *config)
{
    if (config_verify(config) != Ok)
    {
        return ErrorException;
    }

    // AliPubParams_t *AliPubParams;
    // AliPubParamsCreate(&AliPubParams, config);
    //   AliPubParamsInit(AliPubParams);
    // AliPubParamsDelete(AliPubParams);
    return Ok;
}