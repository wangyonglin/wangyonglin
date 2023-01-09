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
#define FileNotFoundException ((ok_t)-5)
#define ExitException ((ok_t)-9)

typedef enum _datatype
{
    STRING,
    INTEGER,
    BOOLEAN
} datatype;

typedef int string;
typedef int integer;
typedef int boolean;

#define positive ((integer)1)
#define negative ((integer)0)
#define invalid ((integer)-1)

// #define STRING ((integer)1)
// #define INTEGER ((integer)2)
// #define BOOLEAN ((integer)3)

#define ENABLED ((boolean)1)
#define DISABLED ((boolean)0)

#define onstart ((integer)1)
#define onstop ((integer)0)
#define onstatus ((integer)-1)
#endif