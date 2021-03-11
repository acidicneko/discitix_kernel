#include "cpu/port.h"
#include "devices/vga.h"
#include "devices/terminal.h"
#include "devices/kbd.h"
#include "utility/log.h"


// cursor cordinates
uint8_t x_pos = 0;
uint8_t y_pos = 0;

// video memory
uint8_t* vidmem = (uint8_t*) 0xB8000;

// default attributes
uint8_t def_fg;
uint8_t def_bg;


void terminal_putchar_col(char c, uint8_t fg, uint8_t bg){
    uint32_t index = (y_pos * 80 + x_pos) * 2;
    
    if(c == '\n'){
        x_pos = 0;
        y_pos++;
    }

    else if(c == '\r')
        x_pos = 0;
    
    else if(c == '\b'){
        if(x_pos == 0){
            x_pos = 79;
            y_pos--;
            terminal_putchar_col(' ', fg, bg);
            x_pos=79;
            y_pos--;
            return;
        }
        x_pos--;
        terminal_putchar_col(' ', fg, bg);
        x_pos--;
    }

    else if(c == '\t')
        x_pos = (x_pos - (x_pos % 8)) + 8;

    else{
        vidmem[index] = c;
        vidmem[index + 1] = (bg << 4) | (fg & 0x0F);
        x_pos++;
    }
    
    if(x_pos >= 80){
        y_pos++;
        x_pos = 0;
    }
    
    if(y_pos >= 25)
        terminal_scroll();
    terminal_update_cursor(x_pos, y_pos);
}

void terminal_putstr_col(const char *str, uint8_t fg, uint8_t bg){
    while(*str!=0){
        terminal_putchar_col(*str, fg, bg);
        str++;
    }
}

void terminal_putchar(char ch){
    terminal_putchar_col(ch , def_fg, def_bg);
}

void terminal_putstr(const char *string){
    terminal_putstr_col(string, def_fg, def_bg);
}

void terminal_scroll(){
    int i;
    for(i = 80*0*2; i <= 80*25*2; i++){
        vidmem[i] = vidmem[i+160];
        i++;
        vidmem[i] = vidmem[i+160];
    }
    for(i = 24*80*2; i <= 25*80*2; i++){
        vidmem[i] = ' ';
        i++;
        vidmem[i] = (def_bg << 4)|(def_fg & 0x0F);
    }
    y_pos = 24;
    x_pos = 0;
}

void terminal_update_cursor(uint8_t x, uint8_t y){
    uint16_t cursorLocation = y * 80 + x;
    outportb(0x3D4, 14);
    outportb(0x3D5, cursorLocation >> 8);
    outportb(0x3D4, 15);                  
    outportb(0x3D5, cursorLocation);
}

void terminal_clear(uint8_t fg, uint8_t bg){
    for(int i = 0; i < 80*25*2; i++){
        vidmem[i] = ' ';
        i++;
        vidmem[i] = (bg << 4)|(fg & 0x0F);
    }
    x_pos = 0;
    y_pos = 0;
    terminal_update_cursor(x_pos, y_pos);
}

uint8_t terminal_fg(){
    return def_fg;
}

uint8_t terminal_bg(){
    return def_bg;
}

uint8_t terminal_getx(){
    return x_pos;
}

uint8_t terminal_gety(){
    return y_pos;
}

void terminal_setx(uint8_t value){
    if(value > 80){
        error("value for X out of range!");
        return;
    }
    x_pos = value;
    terminal_update_cursor(x_pos, y_pos);
}

void terminal_sety(uint8_t value){
    if(value > 80){
        error("value for Y out of range!");
        return;
    }
    y_pos = value;
    terminal_update_cursor(x_pos, y_pos);
}

void info(char *msg){
    terminal_putstr_col("[INFO] ", GREEN, def_bg);
    terminal_putstr(msg);
    terminal_putchar('\n');
}

void error(char *msg){
    terminal_putstr_col("[ERROR] ", RED, def_bg);
    terminal_putstr(msg);
    terminal_putchar('\n');
}

void init_terminal(uint8_t fg, uint8_t bg){
    def_bg = bg;
    def_fg = fg;
    terminal_clear(fg, bg);
    log(INFO, "VGA driver Loaded\n");
}