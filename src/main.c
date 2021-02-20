#include "cpu/dt.h"
#include "devices/vga.h"
#include "devices/terminal.h"
#include "devices/console.h"
#include "devices/timer.h"
#include "devices/kbd.h"
#include "klibc/string.h"
#include "misc/version.h"


void kmain(){
    init_terminal(WHITE, BLACK);
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    timer_install();
    keyboard_install();
    print_info();
    char c;
    while(1){
        c = keyboard_read();
        putc(c);
    }
    for(;;){
        asm("hlt");
    }
}