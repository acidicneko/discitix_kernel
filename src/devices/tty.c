#include "devices/framebuffer.h"
#include "devices/tty.h"
#include "utility/log.h"

uint32_t current_fg;    /*current foregorund color*/
uint32_t current_bg;    /*current background color*/

int init_tty(multiboot_info_t *mbootptr, uint32_t fg, uint32_t bg){ /*initilaise TTY Driver*/
    int status = init_framebuffer(mbootptr);    /*initlaise Framebuffer driver*/
    current_fg = fg;
    current_bg = bg;
    log(INFO, "TTY Driver loaded\n");           /*notify that TTY driver has been loaded*/
    return status;
}

void tty_putchar_col(char c, uint32_t color){   /*prints a single character*/
    if(c == '\b')                               /*handle a backspace*/
        framebuffer_backspace(current_bg);
    else
        framebuffer_putchar_col(c, color);
}

void tty_putstr_col(const char *str, uint32_t color){
    while(*str != 0){
        framebuffer_putchar_col(*str, color);
        str++;
    }
}

void tty_putchar(char c){
    tty_putchar_col(c, current_fg);
}

void tty_putstr(const char *str){
    tty_putstr_col(str, current_fg);
}

void tty_setx(uint32_t value){
    if(tty_cursor(2) == 1) return;
    framebuffer_setx(value);
}

void tty_sety(uint32_t value){
    if(tty_cursor(2) == 1) return;
    framebuffer_sety(value);
}

uint32_t tty_getx(){
    return (uint32_t)framebuffer_getx();
}

uint32_t tty_gety(){
    return (uint32_t)framebuffer_gety();
}

uint32_t tty_fg(){
    return current_fg;
}

uint32_t tty_bg(){
    return current_bg;
}

void tty_clear(uint32_t color){
    framebuffer_cls(color);
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