#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>

void terminal_putchar(char ch);
void terminal_putchar_col(char c, uint8_t fg, uint8_t bg);
void terminal_putstr(const char *string);
void terminal_putstr_col(const char *str, uint8_t fg, uint8_t bg);
void terminal_scroll();
void terminal_update_cursor(uint8_t x, uint8_t y);
void terminal_clear(uint8_t fg, uint8_t bg);
char terminal_getchar();
uint8_t terminal_fg();
uint8_t terminal_bg();
uint8_t terminal_getx();
uint8_t terminal_gety();
void terminal_setx(uint8_t value);
void terminal_sety(uint8_t value);
void info(char *msg);
void error(char *msg);
void init_terminal(uint8_t fg, uint8_t bg);

#endif