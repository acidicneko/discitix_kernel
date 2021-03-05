#include "cpu/multiboot.h"
#include "mm/bitmap.h"


bool find(bitmap_t* bitmap, uint32_t index){
    uint32_t byte_index = index / 8;
    uint8_t bit_index = index % 8;
    uint8_t bit_indexer = 0b10000000 >> bit_index;
    if((bitmap->buffer[byte_index] & bit_indexer) > 0) return true;
    return false;
}

void set(bitmap_t* bitmap, uint32_t index, bool value){
    uint32_t byte_index = index / 8;
    uint8_t bit_index = index % 8;
    uint8_t bit_indexer = 0b10000000 >> bit_index;
    bitmap->buffer[byte_index] &= ~bit_indexer;
    if(value) bitmap->buffer[byte_index] |= bit_indexer;
}