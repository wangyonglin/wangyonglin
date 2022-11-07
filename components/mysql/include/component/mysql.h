#if !defined(__COMPONENT_MYSQL_H__)
#define __COMPONENT_MYSQL_H__

#include <mysql/mysql.h>
typedef struct
{
    char *host;
    int port;
    char *username;
    char *password;
    char *database;
} mysql_config_t;

#endif