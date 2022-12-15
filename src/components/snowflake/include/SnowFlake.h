/*
 * 版权属于：yitter(yitter@126.com)
 * 开源地址：https://gitee.com/yitter/idgenerator
 */
#pragma once

#include "IdGenOptions.h"
#include "common.h"

TAP_DLLEXPORT
extern void TAP_STDCALL SetIdGenerator(IdGeneratorOptions options);

TAP_DLLEXPORT
extern void TAP_STDCALL SetWorkerId(uint32_t workerId);

TAP_DLLEXPORT
extern int64_t TAP_STDCALL NextId();
/**
 * @brief 
 * 
 * @param workerId      /// 机器码，必须由外部设定，最大值 2^WorkerIdBitLength-1
 * @param method        /// 雪花计算方法,（1-漂移算法|2-传统算法），默认1
 * @param SeqBitLength  /// 序列数位长，默认值6，取值范围 [3, 21]（要求：序列数位长+机器码位长不超过22）
 */
void SnowFlake_create(uint32_t workerId, uint8_t method, uint8_t SeqBitLength);
/**
 * @brief       /// 生成ID： 
 *                           5745600032343070
 *                           5745600032343071
 *                           5745600032343072
 * @return int64_t 
 */
int64_t SnowFlake_IdGenerator();

/**
 * @brief
 * #include "SnowFlake.h"

        int main()
        {

            SnowFlake_initializing(1, 1, 10);
            for (int i = 0; i < 100; i++)
            {
                int64_t id = SnowFlake_generator();
                printf("\tid%d  :   %ld\n",i, id);
            }
        }
        id89  :   5745600032343074
        id90  :   5745600032343075
        id91  :   5745600032343076
        id92  :   5745600032343077
        id93  :   5745600032343078
        id94  :   5745600032343079
        id95  :   5745600032343080
        id96  :   5745600032343081
        id97  :   5745600032343082
        id98  :   5745600032343083
        id99  :   5745600032343084
 */