#include "devices/tty.h"
#include "utility/log.h"

display_driver_t* current_driver = NULL;

void init_tty(display_driver_t* driver, uint8_t fg, uint8_t bg){
    current_driver = driver;
    current_driver->init(fg, bg);
    log(INFO, "TTY Driver loaded\n\tDisplay driver: %s\n", current_driver->driver_name);
}

void tty_putchar_col(char c, uint8_t fg, uint8_t bg){
    current_driver->putchar_col(c, fg, bg);
}

void tty_putstr_col(const char *str, uint8_t fg, uint8_t bg){
    while(*str != 0){
        current_driver->putchar_col(*str, fg, bg);
        str++;
    }
}

void tty_putchar(char c){
    tty_putchar_col(c, current_driver->getfg(), current_driver->getbg());
}

void tty_putstr(const char *str){
    tty_putstr_col(str, current_driver->getfg(), current_driver->getbg());
}

void tty_setx(uint32_t value){
    current_driver->setx(value);
}

void tty_sety(uint32_t value){
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

void tty_clear(uint8_t fg, uint8_t bg){
    current_driver->cls(fg, bg);
}