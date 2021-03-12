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
    
    puts("Launching kshell...\n");
    shell_entry();
    
    for(;;){
        asm("sti;hlt");
    }
}