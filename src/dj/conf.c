#include <dj/conf.h>

/*设置输出颜色*/
#define red_color "\E[1;31m"
#define color_suffix "\E[0m"

/**
 * @brief
 *
 * @param conf
 * @param filename
 * @return conf_t*
 */
conf_t *conf_crt(conf_t **conf, const char *filename)
{

    if ((*conf) = (conf_t *)malloc(sizeof(conf_t)))
    {
        memset((*conf), 0x00, sizeof(sizeof(conf_t)));
        FILE *fp;
        // 1. Read and parse toml file
        if (fp = fopen(filename, "r"))
        {

            if ((*conf)->conf = toml_parse_file(fp, (*conf)->errbuf, sizeof((*conf)->errbuf)))
            {
                fclose(fp);
                return (*conf);
            }
            else
            {
                fclose(fp);
                fprintf(stderr, "ERROR:cannot parse - ", (*conf)->errbuf);
            }
        }
        else
        {
            fprintf(stderr, "ERROR:cannot open %s - ", filename, strerror(errno));
        }
        free(*conf);
    }
    return NULL;
}
/**
 * @brief
 *
 * @param conf
 */
void conf_del(conf_t *conf)
{
    if (conf)
    {
        if (conf->conf)
        {
            toml_free(conf->conf);
        }
        free(conf);
    }
}

/**
 * @brief
 *
 * @param conf
 * @param tablekey
 * @param datumkey
 * @param out
 * @return trun_of_t
 */
int conf_string_in(conf_t *conf, const char *tablekey, const char *datumkey, char **call)
{
    if (conf->conf)
    {

        toml_table_t *table = NULL;
        toml_datum_t datum;
        // 2. Traverse to a table.
        if (table = toml_table_in(conf->conf, tablekey))
        {
            // 3. Extract values
            if ((datum = toml_string_in(table, datumkey)).ok)
            {

                string_crt(call, datum.u.s, strlen(datum.u.s));
                free(datum.u.s);
                return (1);
            }
            else
            {
                fprintf(stderr, "cannot read [%s.%s]", tablekey, datumkey);
            }
        }
        else
        {
            fprintf(stderr, "missing [%s]", tablekey);
        }
    }

    return 0;
}

/**
 * @brief 
 * 
 * @param conf 
 * @param tablekey 
 * @param datumkey 
 * @param call 
 * @return int 
 */
int conf_int64_in(conf_t *conf, const char *tablekey, const char *datumkey, int64_t *call)
{
    if (conf)
    {

        toml_table_t *table = NULL;
        toml_datum_t datum;
        // 2. Traverse to a table.
        if (table = toml_table_in(conf->conf, tablekey))
        {
            // 3. Extract values
            if ((datum = toml_int_in(table, datumkey)).ok)
            {
                *call = datum.u.i;
                return (1);
            }
            else
            {
                fprintf(stderr, "cannot read [%s.%s]", tablekey, datumkey);
            }
        }
        else
        {
            fprintf(stderr, "missing [%s]", tablekey);
        }
    }

    return (0);
}
