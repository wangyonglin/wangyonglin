#ifndef __WANGYONGLIN_CORE_H__
#define __WANGYONGLIN_CORE_H__
#include <wangyonglin/linux.h>
typedef int levels_t;
#define On ((levels_t)1)
#define Off ((levels_t)0)
#define None ((levels_t)-1)

typedef int ok_t;
#define ok ((ok_t)1)
#define successful ((ok_t)1)
#define failed ((ok_t)0)
#define none ((ok_t)-1)
#define null ((ok_t)-2)

#define disabled ((int)0)
#define enabled ((int)1)




#endif /**__WANGYONGLIN_CORE_H__**/