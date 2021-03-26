#include "devices/kbd.h"
#include "devices/tty.h"
#include "klibc/stdio.h"
#include "klibc/string.h"

void putuhex(uint32_t n){
    int tmp;

    puts("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4){
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0){
            continue;
        }
    
        if (tmp >= 0xA){
            noZeroes = 0;
            putc(tmp-0xA+'a' );
        }
        else{
            noZeroes = 0;
            putc( tmp+'0' );
        }
    }
    tmp = n & 0xF;
    if (tmp >= 0xA){
        putc(tmp-0xA+'a');
    }
    else{
        putc(tmp+'0');
    }

}

void putihex(int n){
    int tmp;

    puts("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4){
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0){
            continue;
        }
    
        if (tmp >= 0xA){
            noZeroes = 0;
            putc(tmp-0xA+'a' );
        }
        else{
            noZeroes = 0;
            putc( tmp+'0' );
        }
    }
    tmp = n & 0xF;
    if (tmp >= 0xA){
        putc(tmp-0xA+'a');
    }
    else{
        putc(tmp+'0');
    }

}

void putc(char ch){
    tty_putchar(ch);
}

void puts(const char *str){
    tty_putstr(str);
}

void putint(int integer){
    char temp[15];
    itoa(integer, temp);
    puts(temp);
}

void putuint(uint32_t unsignedInt){
    char temp[] = "";
    utoa(unsignedInt, temp);
    puts(temp);
}

void _vsprintf(char *str, va_list arg){
    int integer;
    char character;
    uint32_t unsignedLong;
    uint16_t unsignedShort;
    uint8_t unsignedChar;
    char *string;
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
                case 'x':
                    str++;
                    if(*str == 'c'){
                        character = va_arg(arg, int);
                        putihex((int)character);
                    }
                    else if(*str == 'i'){
                        integer = va_arg(arg, int);
                        putihex(integer);
                    }
                    else if(*str == 'h'){
                        unsignedChar = va_arg(arg, int);
                        putuhex((uint32_t)unsignedChar);
                    }
                    else if(*str == 'u'){
                        unsignedShort = va_arg(arg, int);
                        putuhex((uint32_t)unsignedShort);
                    }
                    else if(*str == 'U'){
                        unsignedLong = va_arg(arg, uint32_t);
                        putuhex(unsignedLong);
                    }
                    else {
                        putc(*str);
                    }
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
}

void kprintf(char *str, ...){
    va_list arg;
    va_start(arg, str);
    _vsprintf(str, arg);
    va_end(arg);
}

void gets(char *buffer){
    char c;
    int i = 0;
    tty_cursor(1);
    while(1){
        c = keyboard_read();
        if(c == '\n'){
            buffer[i] = 0;
            putc('\n');
            break;
        }
        else if(c == '\b'){
            if(i > 0){
                putc('\b');
                i = i - 1;
            }
        }
        else{
            putc(c);
            buffer[i] = c;
            i++;
        }
    }
    tty_cursor(0);
}