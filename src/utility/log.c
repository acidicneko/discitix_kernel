#include "utility/log.h"
#include "devices/tty.h"
#include "klibc/stdio.h"

void log(int status, char *str, ...){
    va_list arg;
    va_start(arg, str);
    if(status == 1) tty_putstr_col("[INFO] ", 0x00ff08);
    else if(status == 2) tty_putstr_col("[ERROR] ", 0xff1100);
    _vsprintf(str, arg);
    va_end(arg);
}