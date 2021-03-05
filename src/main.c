#include "cpu/dt.h"
#include "cpu/multiboot.h"
#include "devices/vga.h"
#include "devices/terminal.h"
#include "devices/console.h"
#include "devices/timer.h"
#include "devices/kbd.h"
#include "klibc/string.h"
#include "mm/memory.h"
#include "misc/version.h"
#include "misc/shell.h"

void kmain(multiboot_info_t* mboot){
    init_terminal(WHITE, BLACK);
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    timer_install();
    keyboard_install();
    print_memory(mboot);
    print_info();

    puts("Launching kshell...\n");
    shell_entry();

    for(;;){
        asm("sti;hlt");
    }
}