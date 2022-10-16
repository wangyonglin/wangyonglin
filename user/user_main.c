#include <stdio.h>
#include <stdlib.h>
#include <system/string.h>
#include <system/config.h>
#include <system/daemon.h>
#include <https_server.h>
/*
   STACK_OF(CONF_VALUE) *sect = NULL;
   CONF_VALUE *cnf;
   sect = NCONF_get_section(config->conf->conf, "HTTPS");
   for (int i = 0; i < sk_CONF_VALUE_num(sect); i++)
   {
       cnf = sk_CONF_VALUE_value(sect, i);
       config_log_error(config->log, "name:%s value:%s", cnf->name, cnf->value);
   }
   */

config_t *config;
https_httpd_config_t *httpd_config;
void err_handler(int sig)
{

    if ((SIGINT == sig) | (SIGTERM == sig))
    {
        httpd_cleanup(httpd_config);
        config_deallocate(config);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, err_handler);  // Ctrl + C
    signal(SIGTERM, err_handler); // kill发出的软件终止

    if (config_allocate(&config))
    {
        if (config_initializing(config, argc, argv) == OK_SUCCESS)
        {
            if (config_management(config) != OK_SUCCESS)
            {
                config_deallocate(config);
                return EXIT_SUCCESS;
            }
            httpd_initializing(&httpd_config, "0.0.0.0", 80);
        }
        goto hello_exit;
    }
hello_exit:
    httpd_cleanup(httpd_config);
    config_deallocate(config);
    return EXIT_SUCCESS;
}