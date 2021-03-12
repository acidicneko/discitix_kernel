#ifndef _TTY_H_
#define _TTY_H_

#include <stdint.h>
#include <stddef.h>

typedef struct{
    char *driver_name;
    void (*init)(uint8_t fg, uint8_t bg);
    void (*putchar_col)(char c, uint8_t fg, uint8_t bg);
    void (*cls)(uint8_t fg, uint8_t bg);
    uint32_t (*getx)();
    uint32_t (*gety)();
    uint8_t (*getfg)();
    uint8_t (*getbg)();
    void (*sety)(uint32_t value);
    void (*setx)(uint32_t value);
} display_driver_t;

void init_tty(display_driver_t* driver, uint8_t fg, uint8_t bg);
void tty_putchar_col(char c, uint8_t fg, uint8_t bg);
void tty_putstr_col(const char *str, uint8_t fg, uint8_t bg);
void tty_putchar(char c);
void tty_putstr(const char *str);
void tty_setx(uint32_t value);
void tty_sety(uint32_t value);
uint32_t tty_getx();
uint32_t tty_gety();
uint8_t tty_fg();
uint8_t tty_bg();
void tty_clear(uint8_t fg, uint8_t bg);

#endif /* tty.h */