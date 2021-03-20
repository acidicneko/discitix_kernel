#include "devices/framebuffer.h"
#include "cpu/multiboot.h"
#include "klibc/string.h"
#include "devices/font.h"
#include "utility/log.h"

static uint32_t *framebuffer_buffer = NULL;
static uint32_t framebuffer_width;
static uint32_t framebuffer_height;
static uint32_t framebuffer_bpp;
static uint32_t framebuffer_pitch;

display_driver_t framebuffer_driver = {
    .init = init_framebuffer,
    .putchar_col = framebuffer_putchar_col,
    .cls = framebuffer_cls,
    .getx = framebuffer_getx,
    .gety = framebuffer_gety,
    .setx = framebuffer_setx,
    .sety = framebuffer_sety
};

uint32_t x_cur = 0;
uint32_t y_cur = 0;

void init_framebuffer(multiboot_info_t* mboot){
    framebuffer_buffer = (uint32_t*)mboot->framebuffer_addr;
    framebuffer_bpp = mboot->framebuffer_bpp;
    framebuffer_pitch = mboot->framebuffer_pitch;
    framebuffer_height = mboot->framebuffer_height;
    framebuffer_width = mboot->framebuffer_width;
    strcpy(framebuffer_driver.driver_name, "Framebuffer");
    framebuffer_driver.width = framebuffer_width;
    framebuffer_driver.height = framebuffer_height;
    framebuffer_driver.bpp = framebuffer_bpp;
    framebuffer_driver.buffer_addr = framebuffer_buffer;
}

void framebuffer_putpixel(uint32_t x, uint32_t y, uint32_t color){
    *(uint32_t*)(x + y * framebuffer_width + framebuffer_buffer) = color;
}

void framebuffer_draw_rect(uint32_t startx, uint32_t starty, uint32_t stopx, uint32_t stopy, uint32_t color){
    for(uint32_t i = startx; i <= stopx; i++){
        framebuffer_putpixel(i, starty, color);
    }
    for(uint32_t i = starty; i <= stopy; i++){
        framebuffer_putpixel(stopx, i, color);
    }
    for(uint32_t i = stopx; i >= startx; i--){
        framebuffer_putpixel(i, stopy, color);
    }
    for(uint32_t i = stopy; i >= starty; i--){
        framebuffer_putpixel(startx, i, color);
    }
}

void framebuffer_backspace(uint32_t color){
    x_cur--;
    framebuffer_putchar_col(128, color);
    x_cur--;
}


void framebuffer_putchar_col(char c, uint32_t color){
    uint8_t iy, ix;
    if(c == '\n'){
        x_cur = 0;
        y_cur += 1;
    }
    else if(c == '\r')
        x_cur = 0;
    else if(c == '\t')
        x_cur = (x_cur - (x_cur % 8)) + 8;

    else if(c == '\b')
        framebuffer_backspace(0x000000);

    else{
        for(iy = 0; iy < 8; iy++){
            for(ix = 0; ix < 8; ix++){
                if((font[(uint8_t)c][iy] >> ix) & 1){
                    framebuffer_putpixel(ix + x_cur*GLYPH_WIDTH, iy + y_cur*GLYPH_HEIGHT, color);
                }
            }
        }
        x_cur += 1;
    }
    if(x_cur >= framebuffer_width/8){
        x_cur = 0;
        y_cur += 1;
    }
}

void framebuffer_putstr_col(char *str, uint32_t color){
    while(*str!=0){
        framebuffer_putchar_col(*str, color);
        str++;
    }
}

void framebuffer_cls(uint32_t color){
    framebuffer_draw_rect(0, 0, framebuffer_width, framebuffer_height, color);
    x_cur = 0;
    y_cur = 0;
}

uint32_t framebuffer_getx(){
    return x_cur;
}

uint32_t framebuffer_gety(){
    return y_cur;
}

void framebuffer_setx(uint32_t value){
    x_cur = value;
}

void framebuffer_sety(uint32_t value){
    y_cur = value;
}