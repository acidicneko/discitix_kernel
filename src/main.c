#include "cpu/dt.h"
#include "cpu/multiboot.h"
#include "devices/kbd.h"
#include "devices/tty.h"
#include "devices/timer.h"
#include "devices/vga.h"
#include "klibc/string.h"
#include "mm/bitmap.h"
#include "mm/memory.h"
#include "mm/frame.h"
#include "misc/version.h"
#include "misc/shell.h"
#include "utility/log.h"

void kmain(multiboot_info_t* mboot){
    if(!init_tty(mboot, 0xffffff, 0x000000)){   /*if no framebuffer is found, use VGA to print error message and halt away!*/
        vga_putstr_col("No Framebuffer found!\nFatal Error! Halting the system!", WHITE, BLACK);
        asm("cli;hlt");
    }
    gdt_install();  /*load gdt*/
    idt_install();  /*load idt*/
    isrs_install(); /*load isrs*/
    irq_install();  /*install irqs*/
    init_frame_allocator(mboot);    /*initiliaze physical memory allocator*/
    timer_install();    /*install PIT*/
    keyboard_install(); /*install keyboard*/
    print_info();       /*print kernel information, eg. name, version, licensing, etc*/
    
    puts("Launching kshell...\n");
    shell_entry();      /*launch interactive kshell*/
    
    while(1){           /*if shell exits, handle keyboard input endlessly*/
        char c = keyboard_read();
        putc(c);
    }
    for(;;){
        asm("sti;hlt"); /*enable interrupts and wait*/
    }
}