#ifndef TIMER_H
#define TIMER_H

void timer_handler(struct regs *r);
void timer_install();
void timer_wait(int ticks);
void sleep(int ms);
uint32_t get_ticks();

#endif