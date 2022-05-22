#include <wangyonglin/config.h>
#include <wangyonglin/log.h>


ok_t application(config_t *config,void *args){
    log_write(config,LOG_INFO,"application ok");
    return ok;
}