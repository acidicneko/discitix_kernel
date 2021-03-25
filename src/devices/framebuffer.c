#include "cpu/multiboot.h"
#include "devices/font.h"
#include "devices/framebuffer.h"
#include "klibc/string.h"
#include "mm/frame.h"
#include "utility/log.h"

uint32_t *framebuffer_buffer = NULL;    /*pointer to physical address of framebuffer*/
uint32_t framebuffer_width;             /*these should be self explanatory*/
uint32_t framebuffer_height;
uint32_t framebuffer_bpp;
uint32_t framebuffer_pitch;


uint32_t x_cur = 0;                 /*current position of cursor in X-axis*/
uint32_t y_cur = 0;                 /*same but for Y-axis*/

int init_framebuffer(multiboot_info_t* mboot){  /*initialize the framebuffer*/
    framebuffer_buffer = (uint32_t*)((uintptr_t)mboot->framebuffer_addr);   /*the address of framebuffer*/    
    framebuffer_bpp = mboot->framebuffer_bpp;
    framebuffer_pitch = mboot->framebuffer_pitch;
    framebuffer_height = mboot->framebuffer_height;
    framebuffer_width = mboot->framebuffer_width;
    return 1;
}

void framebuffer_putpixel(uint32_t x, uint32_t y, uint32_t color){  /*this plots a single pixel to the given coordinates with the given color*/
    *(uint32_t*)(x + y * framebuffer_width + framebuffer_buffer) = color;
}

void framebuffer_draw_rect(uint32_t startx, uint32_t starty, uint32_t stopx, uint32_t stopy, uint32_t color){   /*inefficient functions to plot rectangles*/
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

void framebuffer_backspace(uint32_t color){ /*prints a 8x8 glyph on previous glyph*/
    x_cur--;
    framebuffer_putchar_col(128, color);
    x_cur--;
}


void framebuffer_putchar_col(char c, uint32_t color){ /*prints a single char to the screen*/
    uint8_t iy, ix;
    if(c == '\n'){  /*if it is a newline character then increment y and make x = 0*/
        x_cur = 0;
        y_cur += 1;
    }
    else if(c == '\r')  /*if it's a carriage return then place cursor at the beginning of line*/
        x_cur = 0;
    else if(c == '\t')  /*move the cursor forward but to point where the x is divisble by 8*/
        x_cur = (x_cur - (x_cur % 8)) + 8;

    else{               /*algorithm to plot a char on screen*/
        for(iy = 0; iy < 8; iy++){
            for(ix = 0; ix < 8; ix++){
                if((font[(uint8_t)c][iy] >> ix) & 1){
                    framebuffer_putpixel(ix + x_cur*GLYPH_WIDTH, iy + y_cur*GLYPH_HEIGHT, color);
                }
            }
        }
        x_cur += 1;     /*increment x to next column*/
    }
    if(x_cur >= framebuffer_width/GLYPH_WIDTH){ /*if x reaches maximum columns then print a newline*/
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

void framebuffer_cls(uint32_t color){       /*this doesn't works!!*/
    //memset((void*)framebuffer_buffer, color, (framebuffer_height * framebuffer_width) + (uint32_t)framebuffer_buffer);  /*TODO!!*/
    for(uint32_t i = 0; i < (framebuffer_height * framebuffer_width); i++){
        framebuffer_buffer[i] = color;
    }
    x_cur = 0;
    y_cur = 0;
}


/*These are all self-explanatory*/
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