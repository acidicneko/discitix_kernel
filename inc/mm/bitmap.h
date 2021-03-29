#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct{
    uint32_t size;
    uint8_t* buffer;
} bitmap_t;

bool find(bitmap_t* bitmap, uint32_t index);
void set(bitmap_t* bitmap, uint32_t index, bool value);

#endif