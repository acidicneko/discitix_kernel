#include "mm/frame.h"
#include "mm/memory.h"
#include "klibc/string.h"
#include "klibc/stdio.h"
#include "utility/log.h"
#include "devices/tty.h"

bitmap_t page_bitmap;

uint32_t free_memory = 0;
uint32_t reserved_memory = 0;
uint32_t used_memory = 0;

bool initialized = false;

void init_frame_allocator(multiboot_info_t* mbootptr){
    calculate_mem(mbootptr);
    if(initialized == true) return;
    initialized = true;
    
    void* largest_free_mem_seg = NULL;
    uint32_t largest_free_mem_seg_size = 0;

    multiboot_memory_map_t *entry = (multiboot_memory_map_t*)mbootptr->mmap_addr;
    while((uint32_t)entry < mbootptr->mmap_addr + mbootptr->mmap_length){
        if(entry->type == 1){
            if(entry->addr + entry->len > largest_free_mem_seg_size){
                largest_free_mem_seg = (void*)((uintptr_t)entry->addr);
                largest_free_mem_seg_size = entry->addr + entry->len;
            }
        }
        entry = (multiboot_memory_map_t*)((uint32_t)entry + entry->size + sizeof(uint32_t));
    }

    uint32_t mem_size = get_mem_bytes();
    free_memory = mem_size;
    uint32_t bitmap_size = (mem_size / 4096 / 8) + 1;
    init_bitmap(bitmap_size, (uint8_t*)largest_free_mem_seg);
    lock_pages((void*)page_bitmap.buffer, (page_bitmap.size / 4096) + 1);
    
    log(INFO, "Memory Map loaded\n\tKernel start: %xU\tKernel end: %xU\n\tTotal system memory: %U Bytes, %U KB, %U MB\n", kernel_start, end, mem_size, mem_size/1024, mem_size/1024/1024);
}

void init_bitmap(uint32_t bitmap_size, uint8_t* buf_addr){
    page_bitmap.size = bitmap_size;
    page_bitmap.buffer = (uint8_t*)buf_addr;
    for(size_t i = 0; i < bitmap_size; i++){
       set(&page_bitmap, i, false);
    }
}

void free_page(void* address){
    // if(!initialized){
    //     log(ERROR, "please initialize PMM first!\n");
    //     return;
    // }
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
    // if(!initialized){
    //     log(ERROR, "please initialize PMM first!\n");
    //     return;
    // }
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
    // if(!initialized){
    //     log(ERROR, "please initialize PMM first!\n");
    //     return;
    // }
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
    // if(!initialized){
    //     log(ERROR, "please initialize PMM first!\n");
    //     return;
    // }
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
    // if(!initialized){
    //     log(ERROR, "please initialize PMM first!\n");
    //     return NULL;
    // }
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