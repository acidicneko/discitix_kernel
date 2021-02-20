#ifndef DT_H
#define DT_H

#include <stdint.h>

//gdt
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, uint8_t access, uint8_t gran);
extern void gdt_install();

//idt
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();

// isrs' registers
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};
extern void isrs_install();

// irqs
extern void irq_install();
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void send_eoi(int irq_num);

#endif