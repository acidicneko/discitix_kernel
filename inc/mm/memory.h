#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>
#include "cpu/multiboot.h"

void print_memory(multiboot_info_t* mbootptr);
uint32_t get_mem_bytes();
void calculate_mem(multiboot_info_t* mbootptr);

#endif