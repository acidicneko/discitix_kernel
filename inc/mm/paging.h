#ifndef _PAGING_H_
#define _PAGING_H_

#include "cpu/dt.h"
#include <stdint.h>

void init_paging();
void switch_page_dir();
page_t* get_page(uint32_t address, int make, page_dir_t *dir);
void page_fault_handler(struct regs *r);

#endif