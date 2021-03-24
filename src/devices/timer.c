#include "cpu/dt.h"
#include "devices/timer.h"
#include "klibc/string.h"
#include "utility/log.h"

volatile uint32_t timer_ticks = 0;  /*store the ticks passed since boot*/

void timer_handler(struct regs *r){ /*PIT handler*/
    (void)r;
    timer_ticks++;                  /*increment the ticks variable, every time this PIT IRQ is fired*/
}

void timer_install(){               /*installer functon for PIT*/
    irq_install_handler(0, timer_handler);  /*install the handler for IRQ0, PIT*/
    log(INFO, "PIT Installed\n");           /*notify that PIT has been installed*/
}

void timer_wait(int ticks){             /*this waits for until given ticks has been passed*/
    unsigned long eticks;
    eticks = timer_ticks + ticks;

    while(timer_ticks < eticks){
        asm volatile("sti;hlt;cli");
    }
}

uint32_t get_ticks(){
    return timer_ticks;
}

void sleep(int ms){                 /*poor implementation of sleep() in standard C library*/
    int ticks_needed = (ms/100)*18;
    timer_wait(ticks_needed);
}