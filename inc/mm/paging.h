#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include "cpu/dt.h"

typedef struct{
    uint32_t present    : 1;   // Page present in memory
    uint32_t rw         : 1;   // Read-only if clear, readwrite if set
    uint32_t user       : 1;   // Supervisor level only if clear
    uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
    uint32_t dirty      : 1;   // Has the page been written to since last refresh?
    uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
    uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct{
    struct page pages[1024];
} page_table_t;

typedef struct{
    struct page_table *tables[1024];
    uint32_t tablesPhysical[1024];
    uint32_t physicalAddr;
} page_dir_t;

void initialize_paging(uint32_t total_frames, uint32_t ident_addr, uint32_t ident_len);
void switch_page_directory(struct page_directory *new);
struct page *get_page(uint32_t address, int make, struct page_directory *dir);
struct page *get_kernel_page(uint32_t address, int make);
struct page *map_kernel_page(uint32_t address, int make);
void unmap_kernel_page(uint32_t address);
void page_fault(struct regs* r);

#endif