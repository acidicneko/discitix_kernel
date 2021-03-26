#include "klibc/string.h"
#include "klibc/stdio.h"
#include "misc/shell.h"
#include "devices/kbd.h"
#include "devices/tty.h"
#include "devices/timer.h"
#include "devices/vga.h"
#include "misc/version.h"
#include "mm/memory.h"
#include "mm/frame.h"

#include <stddef.h>

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

/*void sysfetch(){
    puts("---------sysfetch---------\n");
    kprintf("kernel: \t%s %d.%d.%d_%s\nshell: \t\tKSH 1.0.0\n", name, version, build_maj, build_min, state);
    uptime();
    kprintf("total memory: \t%U MB\n", get_mem_bytes()/1024/1024);
    for(uint8_t i = 0; i<=7; i++){
        tty_putchar_col(' ', tty_fg(), i);
        tty_putchar_col(' ', tty_fg(), i);
    }
    putc('\n');
    for(uint8_t i = 8; i<=16; i++){
        tty_putchar_col(' ', tty_fg(), i);
        tty_putchar_col(' ', tty_fg(), i);
    }
    putc('\n');
}*/


void execute(int argc, char **argv){
    const char *cmd = argv[0];

    if(strcmp(cmd, "echo")==0){
        int temp = 1;
        while(temp != argc){
            kprintf("%s ", argv[temp]);
            temp++;
        }
        kprintf("\n");
    }
    else if(strcmp(cmd, "sysfetch")==0)
        kprintf("Coming soon!\n");
    
    else if(strcmp(cmd, "uname")==0)
        kprintf("%s %d.%d.%d_%s\nBuild %s\n", name, version, build_maj, build_min, state, __DATE__);

    else if(strcmp(cmd, "halt")==0){
        puts("Halting the system!");
        asm("hlt");
    }

    else if(strcmp(cmd, "uptime")==0)
        uptime();

    else if(strcmp(cmd, "about")==0)
        kprintf("KSH 1.0.0\n");
    
    else if(strcmp(cmd, "clear")==0)
        tty_clear(tty_bg());
    
    else if(strcmp(cmd, "themer")==0){
        if(strcmp("help", argv[1])==0)
            kprintf("Change themes!\navailable themes -\ndefault\nforest\nnight\nruby\n");
        else if(strcmp("default", argv[1])==0){
            tty_setfg(0xffffff);
            tty_setbg(0x000000);
            tty_clear(tty_bg());
        }
        else if(strcmp("forest", argv[1])==0){
            tty_setfg(0xffffff);
            tty_setbg(0x03a88a);
            tty_clear(tty_bg());
        }
        else if(strcmp("night", argv[1])==0){
            tty_setfg(0xffffff);
            tty_setbg(0x06104f);
            tty_clear(tty_bg());
        }
        else if(strcmp("ruby", argv[1])==0){
            tty_setfg(0xffffff);
            tty_setbg(0x960e0e);
            tty_clear(tty_bg());
        }
        else
            kprintf("themer: unknown theme: %s\n", argv[1]);
    }
    
    else if(strcmp(cmd, "mem")==0)
        mem_read();

    else if(strcmp(cmd, "help")==0){
    puts("available commands -> \n\
help - brings out this menu\n\
about - prints current shell version\n\
uname - prints current kernel version\n\
halt - halts the system\n\
clear - clears the screen\n\
uptime - tell how long the system has been running\n\
sysfetch - prints system info\n\
echo - prints the gven arguments\n");
    }
    else
        kprintf("ksh: no such command: %s\n", cmd);
}

void shell_entry(){
    char input[1024];
    char *argv[100];
    int argc;
    while(1){
        puts("\nayush@kernel >> ");
        gets(input);
        argc = 0;
		argv[argc] = strtok(input, "\t ");
		while(argv[argc]) {
			argc++;
			argv[argc] = strtok(0, " \t");
		}
        if(strcmp(argv[0], "exit")==0)
            break;
        execute(argc, argv);
    }
}