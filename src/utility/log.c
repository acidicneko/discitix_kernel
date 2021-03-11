#include "utility/log.h"
#include "devices/terminal.h"
#include "klibc/string.h"
#include "devices/vga.h"


void log(int status, char *str, ...){
    int integer;
    char character;
    uint32_t unsignedLong;
    uint16_t unsignedShort;
    uint8_t unsignedChar;
    char *string;
    va_list arg;
    va_start(arg, str);
    if(status == 1) terminal_putstr_col("[INFO] ", GREEN, terminal_bg());
    else if(status == 2) terminal_putstr_col("[ERROR] ", RED, terminal_bg());
    while(*str!=0){
        if(*str == '%'){
            ++str;
            switch(*str){
                case 'c':
                    character = va_arg(arg, int);
                    putc(character);
                    break;
                case 'i':
                case 'd':
                    integer = va_arg(arg, int);
                    putint(integer);
                    break;
                case 'U':
                    unsignedLong = va_arg(arg, uint32_t);
                    putuint(unsignedLong);
                    break;
                case 'u':
                    unsignedShort = va_arg(arg, int);
                    putuint((uint32_t)unsignedShort);
                    break;
                case 'h':
                    unsignedChar = va_arg(arg, int);
                    putuint((uint32_t)unsignedChar);
                    break;
                case 's':
                    string = va_arg(arg, char *);
                    puts(string);
                    break;
                default:
                    putc(*str);
                    break;
            }
        }
        else{
            putc(*str);
        }
        ++str;
    }
    va_end(arg);
}