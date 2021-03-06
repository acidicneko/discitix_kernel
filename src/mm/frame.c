#include "mm/frame.h"
#include "mm/memory.h"
#include "klibc/string.h"


bitmap_t page_bitmap;

uint32_t free_memory = 0;
uint32_t reserved_memory = 0;
uint32_t used_memory = 0;

bool initialized = false;

void read_multiboot_mem_map_entry(multiboot_info_t* mbootptr){
    if(initialized == true) return;
    puts("\nFrame allocator started\n");
    initialized = true;
    
    void* largest_free_mem_seg = NULL;
    size_t largest_free_mem_seg_size = 0;
    puts("Reading memory map\n");
    multiboot_memory_map_t *entry = (multiboot_memory_map_t*)mbootptr->mmap_addr;
    uint32_t mem_limit = mbootptr->mmap_addr + mbootptr->mmap_length;
    while((uint32_t)entry < mem_limit){
        if(entry->type == 1){
            if((entry->addr + entry->len) > largest_free_mem_seg_size){
                largest_free_mem_seg = (void*)(long unsigned int)entry->addr;
                largest_free_mem_seg_size = entry->addr + entry->len;
            }
        }
        entry = (multiboot_memory_map_t*)((uint32_t)entry + entry->size + sizeof(uint32_t));
    }
    puts("Initializing bitmap\n");
    uint32_t mem_size = get_mem_bytes();
    free_memory = mem_size;
    uint32_t bitmap_size = mem_size / 4096 / 8 + 1;
    init_bitmap(bitmap_size, largest_free_mem_seg);
    //puts("Locking pages\n");
    kprintf("bitmap.size: %U", page_bitmap.size / 4096 + 1);
    lock_pages(&page_bitmap, page_bitmap.size / 4096 + 1);

    puts("Reserving pages\n");
    multiboot_memory_map_t *map = (multiboot_memory_map_t*)mbootptr->mmap_addr;

    while((uint32_t)map < (mbootptr->mmap_addr + mbootptr->mmap_length)){
        if(map->type != 1){
            reserve_pages((void*)(long unsigned int)map->addr, (map->addr + map->len)/4096);
        }
        map = (multiboot_memory_map_t*)((uint32_t)map + map->size + sizeof(uint32_t));
    }
    puts("Finished reading memory map\n");
}

void init_bitmap(size_t bitmap_size, void* buf_addr){
    page_bitmap.size = bitmap_size;
    page_bitmap.buffer = (uint8_t*)buf_addr;
    for(int i = 0; i < bitmap_size; i++){
        *(uint8_t*)(page_bitmap.buffer + i) = 0;
    }
}

void free_page(void* address){
    uint32_t index = (uint32_t)address / 4096;
    if(find(&page_bitmap, index) == false) return;
    set(&page_bitmap, index, false);
    free_memory += 4096;
    used_memory -= 4096;
}

void free_pages(void* address, uint32_t page_count){
    for(uint32_t i = 0; i< page_count; i++){
        free_page((void*)((uint32_t)address + (i*4096)));
    }
}

void lock_page(void* address){
    uint32_t index = (uint32_t)address / 4096;
    if(find(&page_bitmap, index) == true) return;
    set(&page_bitmap, index, true);
    free_memory -= 4096;
    used_memory += 4096;
}

void lock_pages(void* address, uint32_t page_count){
    for(uint32_t i = 0; i< page_count; i++){
        lock_page((void*)((uint32_t)address + (i*4096)));
    }
}

void unreserve_page(void* address){
    uint32_t index = (uint32_t)address / 4096;
    if(find(&page_bitmap, index) == false) return;
    set(&page_bitmap, index, false);
    free_memory += 4096;
    reserved_memory -= 4096;
}

void unreserve_pages(void* address, uint32_t page_count){
    for(uint32_t i = 0; i< page_count; i++){
        unreserve_page((void*)((uint32_t)address + (i*4096)));
    }
}

void reserve_page(void* address){
    uint32_t index = (uint32_t)address / 4096;
    if(find(&page_bitmap, index) == true) return;
    set(&page_bitmap, index, true);
    free_memory -= 4096;
    reserved_memory += 4096;
}

void reserve_pages(void* address, uint32_t page_count){
    for(uint32_t i = 0; i< page_count; i++){
        reserve_page((void*)((uint32_t)address + (i*4096)));
    }
}

uint32_t get_free_memory(){
    return free_memory;
}

uint32_t get_reserved_memory(){
    return reserved_memory;
}

uint32_t get_used_memory(){
    return used_memory;
}