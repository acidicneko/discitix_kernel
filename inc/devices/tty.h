#ifndef _TTY_H_
#define _TTY_H_

#include <stdint.h>
#include <stddef.h>
#include "cpu/multiboot.h"

typedef struct{
    char *driver_name;
    uint32_t width;
    uint32_t height;
    uint32_t bpp;
    uint32_t* buffer_addr;
    void (*init)(multiboot_info_t *mbootptr);
    void (*putchar_col)(char c, uint32_t color);
    void (*cls)(uint32_t color);
    uint32_t (*getx)();
    uint32_t (*gety)();
    uint8_t (*getfg)();
    uint8_t (*getbg)();
    void (*sety)(uint32_t value);
    void (*setx)(uint32_t value);
} display_driver_t;

int init_tty(multiboot_info_t *mbootptr,display_driver_t* driver, uint32_t color);
void tty_putchar_col(char c, uint32_t color);
void tty_putstr_col(const char *str, uint32_t color);
void tty_putchar(char c);
void tty_putstr(const char *str);
void tty_setx(uint32_t value);
void tty_sety(uint32_t value);
uint32_t tty_getx();
uint32_t tty_gety();
uint8_t tty_fg();
uint8_t tty_bg();
void tty_clear(uint32_t color);
int tty_cursor(int status);

#endif /* tty.h */