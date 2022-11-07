#include <component/mysql.h>

// mysql连接
const char *host = "192.168.1.2";
const char *user = "wangyonglin";
const char *pwd = "W@ng0811";
const char *db_name = "mosquitto__db";
int main()
{
    MYSQL *mysql = mysql_init(NULL);
    if (mysql != NULL)
    {
        perror("mysql_init");
        return -1;
    }
    else
    {
        printf("mysql初始化失败\n");
        exit(0);
    }

    mysql = mysql_real_connect(mysql, host, user, pwd, db_name, 0, NULL, 0);
    if (mysql != NULL)
    {
        perror("mysql_connect");
    }
    else
    {
        printf("mysql连接失败");
        exit(0);
    }
    return 0;
}
