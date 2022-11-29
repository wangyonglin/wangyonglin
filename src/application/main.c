#include <developer/program.h>

program_t *program;
ok_t ret;
conf_command_t commands[] = {
    {"serverURI", STRING, offsetof(program_t, serverURI)},
    {"maxInflight", NUMBER, offsetof(program_t, maxInflight)},
    {"error_log_printf", BOOLEAN, offsetof(program_t, error_log_printf)}};
int commands_size = sizeof(commands) / sizeof(commands[0]);
int main(int argc, char *argv[])
{
    ret = program_initializing(&program, 1024, argc, argv);

    conf_mapping(program->conf, (void **)&program, sizeof(program_t), "MQTTALIYUN", commands, commands_size);
    printf("\tserverURI %s\r\n", program->serverURI);
    printf("\tmaxInflight %d\r\n", program->maxInflight);
    printf("\terror_log_printf %d\r\n", program->error_log_printf);
    for (;;)
    {
        sleep(5);
        
    }
    program_cleanup(program);
    return ret;
}