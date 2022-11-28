#include <developer/kernel.h>

kernel_t *kernel;
ok_t ret;
conf_command_t commands[] = {
    {"serverURI", STRING, offsetof(kernel_t, serverURI)},
    {"maxInflight", NUMBER, offsetof(kernel_t, maxInflight)},
    {"error_log_printf", BOOLEAN, offsetof(kernel_t, error_log_printf)}};
int commands_size = sizeof(commands) / sizeof(commands[0]);
int main(int argc, char *argv[])
{
    ret = initializing(&kernel, 1024, argc, argv);

    conf_mapping(kernel->conf, (void **)&kernel, sizeof(kernel_t), "MQTTALIYUN", commands, commands_size);
    printf("\tserverURI %s\r\n", kernel->serverURI);
    printf("\tmaxInflight %d\r\n", kernel->maxInflight);
    printf("\terror_log_printf %d\r\n", kernel->error_log_printf);
    cleanup(kernel);
    return ret;
}