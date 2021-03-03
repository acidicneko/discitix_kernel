#include "cpu/dt.h"
#include "mm/paging.h"
#include "mm/frame.h"
#include "mm/kmalloc_early.h"
#include "devices/terminal.h"
#include "klibc/string.h"
#include "kheap.h"

page_dir_t *kernel_directory;
page_dir_t *current_directory;
extern uint32_t placement_address;

uint8_t initialized = 0;

void identity_map(uint32_t address, uint32_t length) {
    uint32_t curr = address & 0xFFFFF000;
    while(curr < address + length) {
        page_t *p = get_page(curr, 1, kernel_directory);
        p->present = 1;
        p->rw = 1;
        p->user = 0;
        p->frame = curr >> 12;
        curr += 0x1000;
    }
}

void initialize_paging(uint32_t total_frames, uint32_t ident_addr, uint32_t ident_len) {
    init_frame_allocator(total_frames);

    kernel_directory = (page_dir_t *)e_kmalloc_a(sizeof (page_dir_t));
    memset(kernel_directory, 0, sizeof (page_dir_t));
    current_directory = kernel_directory;

    uint32_t i = 0;
    for(i = 0; i < 0xFFFFFFFF;) {
        get_page(i, 1, kernel_directory);
        i += 0x1000 * 1024;
        if(i == 0) {
            break;
        }
    }
    info("Kernel page tables: allocated");
    i = 0;
    while(i < placement_address){
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }
    uint32_t heap_start = disable_early_kmalloc();

    //printf("KHEAP_START: %x\n", heap_start);

    for(i = 0; i < INITIAL_HEAP_PAGE_COUNT; i++) {
        alloc_frame(get_page(heap_start + (i * 0x1000), 1, kernel_directory), 0, 0);
    }

    irq_install_handler(14, page_fault);

    if(ident_addr) {
        identity_map(ident_addr, ident_len);
    }

    switch_page_directory(kernel_directory);
    initialized = 1;

    initialize_kheap(heap_start);
    info("Kheap initialized");
}

void switch_page_directory(page_dir_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}


page_t *get_page(uint32_t address, int make, page_dir_t *dir)
{
    // Turn the address into an index.
    address /= 0x1000;
    // Find the page table containing this address.
    uint32_t table_idx = address / 1024;
    if (dir->tables[table_idx]) // If this table is already assigned
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
        uint32_t tmp;
        if(!initialized)
        {
            dir->tables[table_idx] = (page_t_table *)e_kmalloc_ap(sizeof(page_t_table), &tmp);
        }
        else
        {
            dir->tables[table_idx] = (page_t_table *)kmalloc_ap(sizeof(page_t_table), 1, &tmp);
        }
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else
    {
        return NULL;
    }
}

page_t *get_kernel_page(uint32_t address, int make){
    return get_page(address, make, kernel_directory);
}

page_t *map_kernel_page(uint32_t address, int make){
    page_t *p = get_page(address, make, kernel_directory);
    if(!p) return NULL;
    alloc_frame(p, 0, 0);
    return p;
}

void unmap_kernel_page(uint32_t address)
{
    page_t *p = get_page(address, 0, kernel_directory);
    if(p)
        free_frame(p);
}

void page_fault(strcut regs* r){
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    int present = !(r->err_code & 0x1); // Page not present
    int rw = r->err_code & 0x2;         // Write operation?
    int us = r->err_code & 0x4;         // Processor was in user-mode?
    int reserved = r->err_code & 0x8;   // Overwritten CPU-reserved bits of page entry?
    //int id = r->err_code & 0x10;        // Caused by an instruction fetch?

    error("Page Fault!")
    if (present) {puts("present ");}
    if (rw) {puts("read-only ");}
    if (us) {puts("user-mode ");}
    if (reserved) {puts("reserved ");}
    //printf(") at %x\n", faulting_address);
    //PANIC("Page fault");
}