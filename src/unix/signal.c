#include <unix/signal.h>
#include <unix/string.h>

void unix_signal_error(error_handle_t err)
{
        if (err)
        {
            signal(SIGINT, err);  // Ctrl + C
            signal(SIGTERM, err); // kill发出的软件终止
        }
}

void unix_signal_exit(exit_handle_t ext)
{
   
        if (ext)
        {
            atexit(ext);
        }
}
