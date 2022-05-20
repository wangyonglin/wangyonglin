#ifndef __WANGYONGLIN_CORE_H__
#define __WANGYONGLIN_CORE_H__
typedef int tags_t;
#define On ((tags_t)1)
#define Off ((tags_t)0)
#define None ((tags_t)-1)

typedef int ok_t;
#define ok ((ok_t)1)
#define failed ((ok_t)0)

typedef int activated;
#define disabled ((activated)0)
#define enabled ((activated)1)

typedef struct
{
    void *(*allocate)(size_t size);
    void (*deallocate)(void *pointer);
    void *(*reallocate)(void *pointer, size_t size);
} internal_hooks_t;

static internal_hooks_t global_hooks = {malloc, free, realloc};

#endif /**__WANGYONGLIN_CORE_H__**/