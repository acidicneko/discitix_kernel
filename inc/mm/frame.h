#ifndef _FRAME_H_
#define _FRAME_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "mm/bitmap.h"
#include "cpu/multiboot.h"

extern uint32_t kernel_start;
extern uint32_t end;

void init_frame_allocator(multiboot_info_t* mbootptr);
void init_bitmap(uint32_t bitmap_size, uint8_t* buf_addr);
void free_page(void* address);
void lock_page(void* address);
void unreserve_page(void* address);
void reserve_page(void* address);
void* request_page();

void free_pages(void* address, uint32_t page_count);
void lock_pages(void* address, uint32_t page_count);
void unreserve_pages(void* address, uint32_t page_count);
void reserve_pages(void* address, uint32_t page_count);

uint32_t get_free_memory();
uint32_t get_reserved_memory();
uint32_t get_used_memory();

#endif