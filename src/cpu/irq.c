#include "cpu/dt.h"
#include "cpu/port.h"
#include "utility/log.h"

/*all IRQ handlers*/
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/*pointers to IRQ handlers in C*/
void *irq_routines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/*install a given function as handler of given IRQ*/
void irq_install_handler(int irq, void (*handler)(struct regs *r)){
    irq_routines[irq] = handler;
}

/*nuke the handlers*/
void irq_uninstall_handler(int irq){
    irq_routines[irq] = 0;
}


/*remap the PIC*/
void irq_remap(void){
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

/*Installer function for irqs*/
void irq_install(){
    irq_remap();    /*remap PIC*/
    idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);
    idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
    asm volatile ("sti");   /*we should enable interrupts at this point*/
    log(INFO, "IRQs Installed\n");  /*notify that IRQs have been installed*/
}

/*common function for all IRQs*/
void irq_handler(struct regs *r){
    void (*handler)(struct regs *r);    /*blank IRQ handler function*/
    
    handler = irq_routines[r->int_no - 32];
    if (handler){   /*check if handler is present*/
        handler(r); /*if it is present call it*/
    }
    
    if (r->int_no >= 40){       /*send End of Interrupt command to the PIC*/
        outportb(0xA0, 0x20);   /*slave PIC*/
    }
    outportb(0x20, 0x20);       /*master PIC*/
}