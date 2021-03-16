#include "devices/vesa.h"
#include "cpu/multiboot.h"
#include "klibc/string.h"


static uint32_t *vesa_buffer = NULL;
static uint32_t vesa_width;
static uint32_t vesa_height;
static uint32_t vesa_bpp;
static uint32_t vesa_pitch;

void init_vesa(multiboot_info_t* mboot){
    vesa_buffer = (uint32_t*)mboot->framebuffer_addr;
    vesa_bpp = mboot->framebuffer_bpp;
    vesa_pitch = mboot->framebuffer_pitch;
    vesa_height = mboot->framebuffer_height;
    vesa_width = mboot->framebuffer_width;
    // strcpy(vesa_driver.driver_name, "VESA");
}

// void vesa_putpixel(uint32_t x, uint32_t y, uint32_t color){
//     uint32_t* row = ((uint32_t*)vesa_buffer) + (y*vesa_pitch);
//     row[x] = color;
// }

void vesa_putpixel(uint32_t x, uint32_t y, uint32_t color){
    *(uint32_t*)(x + y * vesa_width + vesa_buffer) = color;
}

void vesa_draw_rect(uint32_t startx, uint32_t starty, uint32_t stopx, uint32_t stopy, uint32_t color){
    for(uint32_t i = startx; i <= stopx; i++){
        vesa_putpixel(i, starty, color);
    }
    for(uint32_t i = starty; i < stopy; i++){
        vesa_putpixel(startx, i, color);
    }
    for(uint32_t i = starty; i <= stopy; i++){
        vesa_putpixel(stopx, i, color);
    }
    for(uint32_t i = startx; i <= stopx; i++){
        vesa_putpixel(i, stopy, color);
    }
}