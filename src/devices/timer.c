#include "cpu/dt.h"
#include "klibc/string.h"
#include "devices/timer.h"
#include "devices/terminal.h"

volatile unsigned int timer_ticks = 0;

void timer_handler(struct regs *r){
    (void)r;
    /* Increment our 'tick count' */
    timer_ticks++;

    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if (timer_ticks % 18 == 0)
    {
        //puts("One second has passed\n");
    }
    //send_eoi(0);
}

void timer_install(){
    /* Installs 'timer_handler' to IRQ0 */
    irq_install_handler(0, timer_handler);
    info("Timer Installed");
}

void timer_wait(int ticks){
    unsigned long eticks;
    eticks = timer_ticks + ticks;

    while(timer_ticks < eticks){
        asm volatile("sti;hlt;cli");
    }
}

void sleep(int ms){
    int ticks_needed = (ms/100)*18;
    timer_wait(ticks_needed);
    asm("sti");
}