#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>
#include "cpu/multiboot.h"

void print_memory(struct multiboot_info* info);

#endif