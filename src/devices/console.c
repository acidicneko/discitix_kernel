#include "devices/console.h"
#include "devices/terminal.h"
#include "devices/vga.h"

char console_buffer[4000];

void console_putchar(char c){
    uint32_t index = (terminal_gety() * 80 + terminal_getx()) * 2;
    if(c == '\b'){
        if(terminal_getx() == 0){
            terminal_setx(79);
            terminal_sety(terminal_gety() - 1);
            console_putchar(' ');
            terminal_setx(79);
            terminal_sety(terminal_gety() - 1);
            return;
        }
        terminal_setx(terminal_getx() - 1);
        console_putchar(' ');
        terminal_setx(terminal_getx() - 1);
    }
    else{
        terminal_putchar(c);
    }
    console_buffer[index] = c;
    console_buffer[index + 1] = (terminal_bg() << 4)|(terminal_fg() & 0x0F);
}

void console_putstr(char *str){
    while(*str!=0){
        console_putchar(*str);
        str++;
    }
}

void start_console(){
    terminal_clear(WHITE, BLACK);
    terminal_setx(0);
    terminal_sety(0);
    console_putstr("Hello from Console!\nSay goodbye to Kernel VGA!\n");
    console_putstr("333");
}