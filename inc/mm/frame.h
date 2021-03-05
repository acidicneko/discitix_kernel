#ifndef _FRAME_H_
#define _FRAME_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "mm/bitmap.h"
#include "cpu/multiboot.h"

typedef struct{
    bitmap_t page_bitmap;
} page_frame_allocator;



#endif