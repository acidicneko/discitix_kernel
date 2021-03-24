#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <stdint.h>
#include <stddef.h>
#include "cpu/multiboot.h"

#define GLYPH_HEIGHT    8
#define GLYPH_WIDTH     8

int init_framebuffer(multiboot_info_t* mboot);
void framebuffer_putpixel(uint32_t x, uint32_t y, uint32_t color);
void framebuffer_draw_rect(uint32_t startx, uint32_t starty, uint32_t stopx, uint32_t stopy, uint32_t color);
void framebuffer_putchar_col(char c, uint32_t color);
void framebuffer_putstr_col(char *str, uint32_t color);
void framebuffer_backspace(uint32_t color);
uint32_t framebuffer_getx();
uint32_t framebuffer_gety();
void framebuffer_setx(uint32_t value);
void framebuffer_sety(uint32_t value);
void framebuffer_cls(uint32_t color);
#endif /* framebuffer.h */