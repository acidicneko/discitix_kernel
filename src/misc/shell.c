#include "klibc/string.h"
#include "misc/shell.h"
#include "devices/kbd.h"

void shell_input(char *buffer){
    char c;
    int i = 0;
    while(1){
        c = keyboard_read();
        if(c == '\n'){
            buffer[i] = 0;
            putc('\n');
            break;
        }
       else{
           putc(c);
           buffer[i] = c;
           i++;
       }
    }
}

int shell_entry(){
    char input[128];
    while(1){
        puts("\n>> ");
        gets(input);
        if(strcmp(input, "about") == 0)
            puts("Wolf X Kernel 1.2.5_alpha\n");
        else if(strcmp(input, "halt") == 0){
            puts("Halting the system!");
            asm("hlt");
        }
    }
}