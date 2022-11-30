#include <developer/program.h>
#include <aliiot.h>
program_t *program;
aliiot_t *aliiot;
ok_t ret;

int main(int argc, char *argv[])
{

    if (ret = program_initializing(&program, 1024, argc, argv) != Ok)
    {
        return ret;
    }

    
    aliiot_initializing(&aliiot, program);
    for(;;){
        sleep(1000);
    }
    program_cleanup(program);
    return ret;
}