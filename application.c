#include <wangyonglin/linux.h>
#include <wangyonglin/config.h>
#include <wangyonglin/daemon.h>
#include <wangyonglin/log.h>
#include <wangyonglin/pid.h>

ok_t application(config_t *config,void *args){
    log_write(config,LOG_INFO,"application ok");
    return ok;
}