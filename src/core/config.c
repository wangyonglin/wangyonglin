#include <wangyonglin/core.h>
#include <wangyonglin/config.h>
static void fatal(const char *msg, const char *msg1)
{
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1 ? msg1 : "");
    exit(1);
}
int config_init(config_t **config, const char *fileconf)
{
    *config = (config_t *)malloc(sizeof(config));
    FILE *fp;
    char errbuf[200];
    int retlen=0;
    // 1. Read and parse toml file
    fp = fopen(fileconf, "r");
    if (!fp)
    {
        fatal("cannot open sample.toml - ", strerror(errno));
    }

    conf_table_t *conf = conf_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if (!conf)
    {
        fatal("cannot parse - ", errbuf);
    }

    // 2. Traverse to a table.
    conf_table_t *log_object = conf_table_in(conf, "log");
    if (!log_object)
    {
        fatal("missing [log]", "");
    }

    // 3. Extract values
    conf_datum_t log_filename = conf_string_in(log_object, "log_filename");
    if (!log_filename.ok)
    {
        fatal("cannot read log.log_filename", "");
    }
    retlen = strlen(log_filename.u.s);
    printf("log_filename: %s[%d]\n", log_filename.u.s, retlen);

    (*config)->log_filename = (char *)malloc(retlen);
    bzero((*config)->log_filename, retlen);
    strcpy((*config)->log_filename, log_filename.u.s);
    free(log_filename.u.s);

    // 4. Extract values
    conf_datum_t log_timestamp_format = conf_string_in(log_object, "log_timestamp_format");
    if (!log_timestamp_format.ok)
    {
        fatal("cannot read log.log_timestamp_format", "");
    }
    retlen = strlen(log_timestamp_format.u.s);
    printf("log_timestamp_format: %s[%d]\n", log_timestamp_format.u.s, retlen);

    (*config)->log_timestamp_format = (char *)malloc(retlen);
    bzero((*config)->log_timestamp_format, retlen);
    strcpy((*config)->log_timestamp_format, log_timestamp_format.u.s);
    free(log_timestamp_format.u.s);

    conf_free(conf);
    return 0;
}

void config_free(config_t *config)
{
    if (config)
    {
        free(config);
    }
}