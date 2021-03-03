#include "mm/memory.h"
#include "klibc/string.h"
#include "devices/terminal.h"


void print_memory(struct multiboot_info* mboot){
    info("Memory Map Loaded");
    kprintf("\tLower system memory: %U MiB\n\tUpper system memory: %U MiB\n", mboot->mem_lower/1024, mboot->mem_upper/1024);
}