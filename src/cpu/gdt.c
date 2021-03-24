#include "cpu/dt.h"
#include "utility/log.h"

/*the gdt_entry structure*/
struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

/*the gdt_ptr we pass to gdt_flush function*/
struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct gdt_entry gdt[3];
struct gdt_ptr gp;

extern void gdt_flush();    /*declare extern gdt_flush so that it can be used in C code*/

void gdt_set_gate(int num, unsigned long base, unsigned long limit, uint8_t access, uint8_t gran)
{
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void gdt_install()
{
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);                /*null descriptor*/
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); /*code segment*/
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); /* data segment */

    gdt_flush(&gp); /*flush gdt pointer*/
    log(INFO, "GDT Loaded\n");  /*notify that gdt has been loaded*/
}