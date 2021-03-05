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
#include "misc/version.h"
#include "misc/shell.h"

uint8_t test_buf[20];

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

    //puts("Launching kshell...\n");
    //shell_entry();

    for(;;){
        asm("sti;hlt");
    }
}