#ifndef _VGA_H_
#define _VGA_H_

#include <stdint.h>

enum VGA_COL {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    WHITE,
    GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    YELLOW,
    BRIGHT_WHITE
};

void vga_putchar_col(char c, uint8_t fg, uint8_t bg);
void vga_putstr_col(char *str, uint8_t fg, uint8_t bg);
void vga_scroll();
void vga_update_cursor(uint8_t x, uint8_t y);
void vga_clear(uint8_t fg, uint8_t bg);
uint8_t vga_fg();
uint8_t vga_bg();
uint32_t vga_getx();
uint32_t vga_gety();
void vga_setx(uint32_t value);
void vga_sety(uint32_t value);
void init_vga(uint8_t fg, uint8_t bg);

#endif