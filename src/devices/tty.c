#include "devices/tty.h"
#include "utility/log.h"

display_driver_t* current_driver = NULL;

int init_tty(multiboot_info_t *mbootptr, display_driver_t* driver, uint32_t color){
    (void)color;
    current_driver = driver;
    current_driver->init(mbootptr);
    log(INFO, "TTY Driver loaded\n\tDisplay driver: %s\n", current_driver->driver_name);
    return 1;
}

void tty_putchar_col(char c, uint32_t color){
    current_driver->putchar_col(c, color);
}

void tty_putstr_col(const char *str, uint32_t color){
    while(*str != 0){
        current_driver->putchar_col(*str, color);
        str++;
    }
}

void tty_putchar(char c){
    tty_putchar_col(c, 0xffffff);
}

void tty_putstr(const char *str){
    tty_putstr_col(str, 0xffffff);
}

void tty_setx(uint32_t value){
    if(tty_cursor(2) == 1) return;
    current_driver->setx(value);
}

void tty_sety(uint32_t value){
    if(tty_cursor(2) == 1) return;
    current_driver->sety(value);
}

uint32_t tty_getx(){
    return (uint32_t)current_driver->getx();
}

uint32_t tty_gety(){
    return (uint32_t)current_driver->gety();
}

uint8_t tty_fg(){
    return current_driver->getfg();
}

uint8_t tty_bg(){
    return current_driver->getbg();
}

void tty_clear(uint32_t color){
    current_driver->cls(color);
}

int tty_cursor(int status){
    static int isLocked = 0;
    if(status == 1)
        isLocked = 1;
    else if(status == 0)
        isLocked = 0;
    
    else if(status > 1)
        return isLocked;
    return isLocked;
}