#include "klibc/string.h"
#include "misc/shell.h"
#include "devices/kbd.h"
#include "devices/terminal.h"

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
}

int shell_entry(){
    char input[128];
    while(1){
        puts("\nayush@kernel >> ");
        shell_input(input);
        if(strcmp(input, "uname") == 0)
            puts("Wolf X Kernel 1.2.5_alpha\nMade by Ayush Yadav\n");
        else if(strcmp(input, "halt") == 0){
            puts("Halting the system!");
            asm("hlt");
        }
        else if(strcmp(input, "help") == 0){
            puts("available commands -> \n\
            help - brings out this menu\n\
            about - prints current shell version\n\
            uname - prints current kernel version\n\
            halt - halts the system\n\
            clear - clears the screen\n");
        }
        else if(strcmp(input, "about") == 0)
            puts("Kernel SHell v1.0.0_aplha\n");
        else if(strcmp(input, "clear") == 0)
            terminal_clear(terminal_fg(), terminal_bg());
        else{
            kprintf("kshell: unknown command: %s\n", input);
        }
    }
}