#include "mm/frame.h"
#include "mm/memory.h"
#include "klibc/string.h"
#include "utility/log.h"
#include "devices/tty.h"

static bitmap_t page_bitmap;

uint32_t free_memory = 0;
uint32_t reserved_memory = 0;
uint32_t used_memory = 0;

bool initialized = false;

extern uint32_t kernel_start;
extern uint32_t end;

void init_frame_allocator(multiboot_info_t* mbootptr){
    calculate_mem(mbootptr);
    if(initialized == true) return;
    initialized = true;
    
    void* largest_free_mem_seg = NULL;
    size_t largest_free_mem_seg_size = 0;

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

    uint32_t mem_size = get_mem_bytes();
    free_memory = mem_size;
    reserve_pages(0, 512*4);
    uint32_t kernel_pages = ((uintptr_t)&end - (uintptr_t)&kernel_start) / 4096 + 1;
    reserve_pages(&kernel_start, kernel_pages);
    uint32_t bitmap_size = (mem_size / 4096 /8) + 1;
    init_bitmap(bitmap_size, largest_free_mem_seg);
    lock_pages(page_bitmap.buffer, page_bitmap.size / 4096 + 1);
    log(INFO, "Memory Map loaded\n\tKernel start: %xU\tKernel end: %xU\n\tTotal system memory: %U Bytes, %U KB, %U MB\n", kernel_start, end, mem_size, mem_size/1024, mem_size/1024/1024);
}

void init_bitmap(size_t bitmap_size, void* buf_addr){
    page_bitmap.size = bitmap_size;
    page_bitmap.buffer = (uint8_t*)buf_addr;
    for(size_t i = 0; i < bitmap_size; i++){
        *(uint8_t*)page_bitmap.buffer[i] = 0;
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

void* request_page(){
    for(uint32_t index = 0; index <page_bitmap.size * 8; index++){
        if(find(&page_bitmap, index) == true) continue;
        lock_page((void*)(index*4096));
        return (void*)(index*4096);
    }
    return NULL;
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