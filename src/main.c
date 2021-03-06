#include "cpu/dt.h"
#include "cpu/multiboot.h"
#include "devices/vga.h"
#include "devices/terminal.h"
#include "devices/console.h"
#include "devices/timer.h"
#include "devices/kbd.h"
#include "klibc/string.h"
#include "mm/bitmap.h"
#include "mm/memory.h"
#include "mm/frame.h"
#include "misc/version.h"
#include "misc/shell.h"

void kmain(multiboot_info_t* mboot){
    init_terminal(WHITE, BLACK);
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    print_memory(mboot);
    timer_install();
    keyboard_install();
    print_info();

    kprintf("Free RAM: %U \nUsed RAM: %U\nReserved RAM: %U", get_free_memory(), get_used_memory(), get_reserved_memory());
    read_multiboot_mem_map_entry(mboot);
    kprintf("Free RAM: %U \nUsed RAM: %U\nReserved RAM: %U", get_free_memory(), get_used_memory(), get_reserved_memory());
    //puts("Launching kshell...\n");
    //shell_entry();

    for(;;){
        asm("sti;hlt");
    }
}