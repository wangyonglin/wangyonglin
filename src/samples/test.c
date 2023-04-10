#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
unsigned char resp[1024];
int pro_http_write_data(void *pBuffer, int nSize, int nMemByte, void *pParam)
{
    if (NULL == pBuffer || NULL == pParam)
    {
        return -1;
    }
    memcpy((char *)pParam, (char *)pBuffer, nSize * nMemByte);
    return nMemByte;
}

void main()
{
    CURL *curl;
    curl = curl_easy_init();
    char dd[] = "Authorization:WECHATPAY2-SHA256-RSA2048 mchid=\"1614902012\",nonce_str=\"3C1720331E2F0A082923252200121D1E\",signature=\"Pq3GDgSKY5Lni0qsoUt5y3DHTc5ERZXngguvXueEjoRHLgsZA47zcSGDcsP/DcX689JnLiEch+fntVpYW6E0/6Fg5v7PZFf30HNOWcid/FcADSf3AsiqzMbBhsx8fheT+RBJE40lNcgPWqWKlADEQcboBkHF5SanHzolk8Oa8Ep5PSaz67J1oCOOo97UwGwa4Sflu2DoIIFs8epfCArpOsidSxGWzeRAy/1742wchNRU4CNiK8iziSzef1XSxawLTwfVmWx5SKOTzF3K9FuBZPZWi3D24mN1QBzxQv7XvdsJ0P1yChnhZ8siYA084Vz6SpqjA7x2dzybkIvu+iiXQA==\",timestamp=\"1679848395\",serial_no=\"199BD35B56B13A3CB38742029388EE12E0A13387\"";
    struct curl_slist *list = NULL;
    list = curl_slist_append(list, "Accept:application/json");
    list = curl_slist_append(list, dd);
    list = curl_slist_append(list, "Content-Type:application/json;charset=utf-8");
    list = curl_slist_append(list, "User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

    // 初始化
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.mch.weixin.qq.com/v3/certificates"); // 设置post address
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 13L);                                         /// 设置超时
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);                                  // 重连超时
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);                                          // 启用时会汇报所有的信息
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, pro_http_write_data);                   // 接收数据回调
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);                                      // 接收数据存放
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);                                    // 验证服务器证书有效性
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2);                                    // 检验证书中的主机名和你访问的主机名一致
                                                                                          // curl_easy_setopt(curl, CURLOPT_CAINFO, “/ oem / data / cacert.pem”);                  // 指定 CA 证书路径
    CURLcode res = curl_easy_perform(curl);                                               // 开启连接

    if (res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    printf("result res = % d, resp is % s\r\n", res, resp); // 打印接收到的网页
    curl_easy_cleanup(curl);
    curl_slist_free_all(list);
}
