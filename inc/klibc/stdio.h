#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

void putint(int);
void putuint(uint32_t);
void puts(const char *str);
void putc(char ch);
void _vsprintf(char *str, va_list arg);
void kprintf(char *str, ...);
void gets();

#endif