#include "klibc/string.h"
#include "misc/shell.h"
#include "devices/kbd.h"
#include "devices/terminal.h"
#include "devices/timer.h"
#include "devices/vga.h"
#include "misc/version.h"
#include "mm/memory.h"
#include "mm/frame.h"

void mem_read(){
    kprintf("Free memory: %U Bytes, %U KBs, %U MBs\n", get_free_memory(), get_free_memory()/1024, get_free_memory()/1024/1024);
    kprintf("Used memory: %U Bytes, %U KBs, %U MBs\n", get_used_memory(), get_used_memory()/1024, get_used_memory()/1024/1024);
    kprintf("Reserved memory: %U Bytes, %U KBs, %U MBs\n", get_reserved_memory(), get_reserved_memory()/1024, get_reserved_memory()/1024/1024);
}

void uptime(){
    int seconds = get_ticks() / 18;
    if(seconds < 60)
        kprintf("kernel uptime: \t%d second(s)\n", seconds);
    else{
        int minutes = 0;
        while(seconds > 59){
            seconds-=60;
            minutes++;
        }
        kprintf("kernel uptime: \t%d minute(s) %d second(s)\n", minutes, seconds);
    }
}

void sysfetch(){
    puts("---------sysfetch---------\n");
    kprintf("kernel: \t%s %d.%d.%d_%s\nshell: \t\tKSH 1.0.0\n", name, version, build_maj, build_min, state);
    uptime();
    kprintf("total memory: \t%U MB\n", get_mem_bytes()/1024/1024);
    for(uint8_t i = 0; i<=7; i++){
        terminal_putchar_col(' ', terminal_fg(), i);
        terminal_putchar_col(' ', terminal_fg(), i);
    }
    putc('\n');
    for(uint8_t i = 8; i<=16; i++){
        terminal_putchar_col(' ', terminal_fg(), i);
        terminal_putchar_col(' ', terminal_fg(), i);
    }
    putc('\n');
}

int shell_entry(){
    char input[128];
    while(1){
        puts("\nayush@kernel >> ");
        gets(input);

        if(strcmp(input, "uname") == 0)
            kprintf("%s %d.%d.%d_%s\n", name, version, build_maj, build_min, state);

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
            clear - clears the screen\n\
            uptime - tell how long the system has been running\n\
            sysfetch - prints system info\n");
        }

        else if(strcmp(input, "about") == 0)
            puts("Kernel SHell v1.0.0_aplha\n");

        else if(strcmp(input, "clear") == 0)
            terminal_clear(terminal_fg(), terminal_bg());

        else if(strcmp(input, "sysfetch") == 0)
            sysfetch();

        else if(strcmp(input, "uptime") == 0)
            uptime();
        
        else if(strcmp(input, "mem_read") == 0)
            mem_read();

        else
            kprintf("kshell: unknown command: %s\n", input);
    }
}