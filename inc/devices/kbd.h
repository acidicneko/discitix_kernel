#ifndef KBD_H
#define KBD_H

#define LEFT_SHIFT_PRESSED 0x2A
#define LEFT_SHIFT_RELEASED 0xAA
#define RIGHT_SHIFT_PRESSED 0x36
#define RIGHT_SHIFT_RELEASED 0xB6
#define CAPS_LOCK 0x3A

#include "cpu/dt.h"
#include "cpu/port.h"
#include <stdint.h>

void keyboard_handler(struct regs *r);
void keyboard_install(void);
char translate(uint8_t scancode);
void handleKey_normal(uint8_t scancode);
void handleKey_arrow(uint8_t scancode);
char keyboard_read();



#endif