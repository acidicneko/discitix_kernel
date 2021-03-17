#include "devices/vesa.h"
#include "cpu/multiboot.h"
#include "klibc/string.h"
#include "devices/font.h"

static uint32_t *vesa_buffer = NULL;
static uint32_t vesa_width;
static uint32_t vesa_height;
static uint32_t vesa_bpp;
static uint32_t vesa_pitch;

display_driver_t vesa_driver = {
    .init = init_vesa,
    .putchar_col = vesa_putchar_col,
};

uint32_t x_cur = 0;
uint32_t y_cur = 0;

void init_vesa(multiboot_info_t* mboot){
    vesa_buffer = (uint32_t*)mboot->framebuffer_addr;
    vesa_bpp = mboot->framebuffer_bpp;
    vesa_pitch = mboot->framebuffer_pitch;
    vesa_height = mboot->framebuffer_height;
    vesa_width = mboot->framebuffer_width;
    strcpy(vesa_driver.driver_name, "VESA");
}

void vesa_putpixel(uint32_t x, uint32_t y, uint32_t color){
    *(uint32_t*)(x + y * vesa_width + vesa_buffer) = color;
}

void vesa_draw_rect(uint32_t startx, uint32_t starty, uint32_t stopx, uint32_t stopy, uint32_t color){
    for(uint32_t i = startx; i <= stopx; i++){
        vesa_putpixel(i, starty, color);
    }
    for(uint32_t i = starty; i <= stopy; i++){
        vesa_putpixel(stopx, i, color);
    }
    for(uint32_t i = stopx; i >= startx; i--){
        vesa_putpixel(i, stopy, color);
    }
    for(uint32_t i = stopy; i >= starty; i--){
        vesa_putpixel(startx, i, color);
    }
}

void vesa_putchar_col(char c, uint32_t color){
    uint8_t iy, ix;
    if(c == '\n'){
        x_cur = 0;
        y_cur+=8;
    }
    else if(c == '\r')
        x_cur = 0;
    else if(c == '\t')
        x_cur = (x_cur - (x_cur % 8)) + 8;
    else{
        for(iy = 0; iy < 8; iy++){
            for(ix = 0; ix < 8; ix++){
                if((font[(uint8_t)c][iy] >> ix) & 1){
                    vesa_putpixel(ix + x_cur, iy + y_cur, color);
                }
            }
        }
        x_cur += 8;
    }
    if(x_cur >= 1024){
        x_cur = 0;
        y_cur+=8;
    }
}

void vesa_putstr_col(char *str, uint32_t color){
    while(*str!=0){
        vesa_putchar_col(*str, color);
        str++;
    }
}