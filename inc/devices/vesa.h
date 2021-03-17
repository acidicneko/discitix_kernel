#ifndef _VESA_H_
#define _VESA_H_

#include <stdint.h>
#include <stddef.h>
#include "devices/tty.h"
#include "cpu/multiboot.h"
extern display_driver_t vesa_driver;

void init_vesa(multiboot_info_t* mboot);
void vesa_putpixel(uint32_t x, uint32_t y, uint32_t color);
void vesa_draw_rect(uint32_t startx, uint32_t starty, uint32_t stopx, uint32_t stopy, uint32_t color);
void vesa_putchar_col(char c, uint32_t color);
void vesa_putstr_col(char *str, uint32_t color);

#endif /* vesa.h */