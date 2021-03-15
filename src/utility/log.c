#include "utility/log.h"
#include "devices/tty.h"
#include "klibc/string.h"
#include "devices/vga.h"

void log(int status, char *str, ...){
    va_list arg;
    va_start(arg, str);
    if(status == 1) tty_putstr_col("[INFO] ", GREEN, tty_bg());
    else if(status == 2) tty_putstr_col("[ERROR] ", RED, tty_bg());
    _vsprintf(str, arg);
    va_end(arg);
}