#if !defined(DEVELOPER_PUBLIC_H)
#define DEVELOPER_PUBLIC_H
typedef int ok_t;
#define Ok ((ok_t)0)
#define DoneException ((ok_t)1)
#define NoneException ((ok_t)0)
#define NullPointerException ((ok_t)-1)
#define ErrorException ((ok_t)-2)
#define ArgumentException ((ok_t)-3)
#define UnknownException ((ok_t)-4)

typedef int boolean;
#define enabled ((boolean)1)
#define disabled ((boolean)0)
#endif