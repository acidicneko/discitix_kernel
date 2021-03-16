#include "cpu/dt.h"
#include "cpu/multiboot.h"
#include "devices/tty.h"
#include "devices/timer.h"
#include "devices/kbd.h"
#include "klibc/string.h"
#include "mm/bitmap.h"
#include "mm/memory.h"
#include "mm/frame.h"
#include "misc/version.h"
#include "misc/shell.h"
#include "utility/log.h"
#include "devices/vga.h"
#include "devices/vesa.h"

void kmain(multiboot_info_t* mboot){
    init_tty(&vga_driver, WHITE, BLACK);
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    read_multiboot_mem_map_entry(mboot);
    timer_install();
    keyboard_install();
    print_info();
    init_vesa(mboot);
    for(int i = 100; i < 300; i++){
        vesa_putpixel(10, i, 0xffffff);
    }
    for(int i = 10; i < 300; i++){
        vesa_putpixel(i, 100, 0xffffff);
    }
    for(int i = 100; i < 300; i++){
        vesa_putpixel(300, i, 0xffffff);
    }
    for(int i = 10; i <= 300; i++){
        vesa_putpixel(i, 300, 0xffffff);
    }
    //puts("Launching kshell...\n");
    //shell_entry();
    
    while(1){
        char c = keyboard_read();
        putc(c);
    }
    for(;;){
        asm("sti;hlt");
    }
}