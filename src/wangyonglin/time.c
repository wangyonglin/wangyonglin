#include <wangyonglin/time.h>
#include <wangyonglin/string.h>

// buffer_t timestamp()
// {

//     buffer_t buffer = buffer_null_command;
//     char tmp[80] = {0};
//     UtcTime result;
//     time_t rawtime;
//     time(&rawtime);
//     result = unix32_to_UTC(rawtime);
//     sprintf(tmp, "%04d-%02d-%02dT%02d:%02d:%02dZ", result.year, result.month, result.day,
//             result.hour, result.minute, result.second); // 以年月日_时分秒的形式表示当前时间
//     buffer.outstring = buffer_outstring(tmp, strlen(tmp));
//     buffer.outlength = strlen(buffer.outstring);
//     return buffer;
// }

