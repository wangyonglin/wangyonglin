#include <MainOption.h>
#include <getopt.h>
#include <config.h>
#define STRINTG_BUFFER_MAX 64

int opt;
const char *short_options = "s:c:l:i:dv";
int option_index = 0;
struct option long_options[] = {
    {"daemon", no_argument, 0, 'd'},
    {"config", optional_argument, 0, 'c'},
    {"log", optional_argument, 0, 'l'},
    {"start", required_argument, 0, 's'},
    {"pid", optional_argument, 0, 'i'},
    {"version", no_argument, 0, 'v'},
    {0, 0, 0, 0}};

#define STRING_MALLOC_MAX 512

Boolean MainOptionCreate(MainOption **mainOption)
{
    if (ObjectCreate((void **)mainOption, sizeof(MainOption)))
    {
        (*mainOption)->daemonize = Boolean_false;
        (*mainOption)->enabled = Boolean_true;
        (*mainOption)->confFilename = Stringex_null;
        StringexResetting(&(*mainOption)->confFilename, PACKAGE_CONF_MAIN_FILENAME, strlen(PACKAGE_CONF_MAIN_FILENAME));
        StringexCreate(&(*mainOption)->logFilename, PACKAGE_CONF_ZLOG_FILENAME, strlen(PACKAGE_CONF_ZLOG_FILENAME));
        return (*mainOption);
    }
    return (*mainOption) = NULL;
}
void MainOptionDelete(MainOption *mainOption)
{
    if (mainOption)
    {
        StringexDelete(mainOption->confFilename);
        StringexDelete(mainOption->logFilename);
        ObjectDelete(mainOption);
    }
}
Boolean MainOptionInit(MainOption *mainOption, int argc, char *argv[])
{
    Boolean err = Boolean_false;
    if (mainOption)
    {
        while (-1 != (opt = getopt_long(argc, argv, short_options, long_options, &option_index)))
        {
            switch (opt)
            {
            case 'c':
                StringexResetting(&mainOption->confFilename, optarg, strlen(optarg));
                break;
            case 'l':
                StringexResetting(&mainOption->logFilename, optarg, strlen(optarg));
                break;
            case 's':
                if (!strcmp(optarg, "start"))
                {
                    mainOption->enabled = Boolean_true;
                }
                else if (!strcmp(optarg, "stop"))
                {
                    mainOption->enabled = Boolean_false;
                }
                else if (!strcmp(optarg, "status"))
                {
                    mainOption->enabled = Boolean_invalid;
                }
                else
                {
                    fprintf(stderr, "starting params error[%s]", optarg);
                    return err;
                }
                break;
            case 'd':
                mainOption->daemonize = Boolean_true;
                break;
            default:
                return err;
                break;
            }
        }

        return Boolean_true;
    }

    return err;
}
