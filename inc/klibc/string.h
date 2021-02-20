#ifndef _STRING_H_
#define _STRING_H_

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

int strlen(const char* str);
uint16_t *memsetw(uint16_t *dest, uint16_t val, size_t count);
uint8_t *memcpy(uint8_t *dest, const uint8_t *src, size_t n);
void* memset(void* bufptr, int value, size_t size);
char *strcpy(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
char *strrev(char *s);
char *strcat(char *d, const char *s);
char *strtok(char *s, const char *delim);
const char *strchr(const char *s, char ch);

uint32_t dgcount(int num);
void itoa(int num, char *dest);
void puts(const char *str);
void putc(char ch);
void kprintf(char *str, ...);
void gets();
#endif