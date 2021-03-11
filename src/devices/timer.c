#include "cpu/dt.h"
#include "klibc/string.h"
#include "devices/timer.h"
#include "utility/log.h"

volatile uint32_t timer_ticks = 0;

void timer_handler(struct regs *r){
    (void)r;
    timer_ticks++;
}

void timer_install(){
    irq_install_handler(0, timer_handler);
    log(INFO, "PIT Installed\n");
}

void timer_wait(int ticks){
    unsigned long eticks;
    eticks = timer_ticks + ticks;

    while(timer_ticks < eticks){
        asm volatile("sti;hlt;cli");
    }
}

uint32_t get_ticks(){
    return timer_ticks;
}

void sleep(int ms){
    int ticks_needed = (ms/100)*18;
    timer_wait(ticks_needed);
    //asm("sti");
}