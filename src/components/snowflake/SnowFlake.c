/*
 * 版权属于：yitter(yitter@126.com)
 * 开源地址：https://github.com/yitter/idgenerator
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "SnowFlake.h"
#include "IdGenerator.h"

extern void SetIdGenerator(IdGeneratorOptions options)
{
    SetOptions(options);
}

extern void SetWorkerId(uint32_t workerId)
{
    IdGeneratorOptions options = BuildIdGenOptions(workerId);
    SetIdGenerator(options);
}

extern int64_t NextId()
{
    return GetIdGenInstance()->NextId();
    //    IdGenerator *generator = GetIdGenInstance();
    //    uint64_t id = generator->NextId();
    //    free(generator);
    //    return id;
}

void SnowFlake_create(uint32_t workerId, uint8_t method, uint8_t SeqBitLength)
{

    IdGeneratorOptions options = BuildIdGenOptions(1);
    options.Method = method;
    options.WorkerId = workerId;
    options.SeqBitLength = 10;
    SetIdGenerator(options);
}

int64_t SnowFlake_IdGenerator()
{
    return GetIdGenInstance()->NextId();
    //    IdGenerator *generator = GetIdGenInstance();
    //    uint64_t id = generator->NextId();
    //    free(generator);
    //    return id;
}
int64_t SnowFlake_IdGenerator_toString(char **pointer)
{
    int64_t id = GetIdGenInstance()->NextId();
    if ((*pointer) = malloc(sizeof(char) * 12))
    {
        memset((*pointer), 0x00, sizeof(char) * 12);
        sprintf((*pointer), "%ld", id);
    }
    return id;
}