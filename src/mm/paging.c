#include "mm/paging.h"

typedef struct{
    uint32_t size;
    uint32_t accessed;
    uint32_t user;
    uint32_t rw;
    uint32_t present;
} page_t;

typedef struct{
    page_t pages[1024];
} page_table_t;

typedef struct{
    page_table_t *table[1024];
    uint32_t physical_table[1024];
    uint32_t physical_address;
} page_dir_t;

