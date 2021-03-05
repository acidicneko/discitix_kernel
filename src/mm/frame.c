#include "mm/frame.h"

uint32_t free_memory;
uint32_t reserved_memory;
uint32_t used_memory;

bool initialized = false;

void read_multiboot_mem_map_entry(multiboot_info_t* mbootptr){
    if(initialized) return;
    initialized = true;
    
    void largest_free_mem_seg = NULL;
    size_t largest_free_mem_seg_size = 0;
    multiboot_memory_map_t *entry = (multiboot_memory_map_t*)mbootptr->mmap_addr;
    uint32_t mem_limit = mbootptr->mmap_addr + mbootptr->mmap_length;
    while((uint32_t)entry < mem_limit){
        if((entry->addr + entry->len) > 0)
        // TODO: AFTER 10 days
        entry = (multiboot_memory_map_t*)((uint32_t)entry + entry->size + sizeof(uint32_t));
    }
}