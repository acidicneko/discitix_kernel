#include "devices/terminal.h"
#include "devices/kbd.h"
#include "klibc/string.h"

// keyboard buffer handling
char buf_char;
uint8_t last_scancode;

// shift and caps flags
static uint8_t isShift = 0;
static uint8_t caps = 0;

static int irq_done = 0;

const char keyMap_normal[58] = {
  0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0','-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
  'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
};

const char keyMap_shift[58] = {
  0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')','_', '+', '\b',
  '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|',
  'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ',
};

void keyboard_handler(struct regs *r)
{
    (void)r;
    uint8_t scancode;
    scancode = inportb(0x60);
    switch(last_scancode){
      case 0xE0:
        handleKey_arrow(scancode);
        break;
      default:
        handleKey_normal(scancode);
        break;
    }
    irq_done = 1;
}


char translate(uint8_t scancode){
  if(scancode > 58)
    return 0;

  if(isShift == 1)
    return keyMap_shift[scancode];

  if(caps == 1){
    if((int)keyMap_normal[scancode] >= 97 && (int)keyMap_normal[scancode] <= 122){
      return (int)keyMap_normal[scancode] - 32;
    }
  }
  return keyMap_normal[scancode];
}

void handleKey_arrow(uint8_t scancode){
  switch(scancode){
    case 0x48:
      terminal_sety(terminal_gety() - 1);
      break;
    case 0x50:
      terminal_sety(terminal_gety() + 1);
      break;
    case 0x4B:
      terminal_setx(terminal_getx() - 1);
      break;
    case 0x4D:
      terminal_setx(terminal_getx() + 1);
      break;
  }

  last_scancode = scancode;
}

void handleKey_normal(uint8_t scancode){
  switch(scancode){
    case LEFT_SHIFT_PRESSED:
      isShift = 1;
      return;
    case LEFT_SHIFT_RELEASED:
      isShift = 0;
      return;
    case RIGHT_SHIFT_PRESSED:
      isShift = 1;
      return;
    case RIGHT_SHIFT_RELEASED:
      isShift = 0;
      return;
    case CAPS_LOCK:
      if(caps == 0)
        caps = 1;
      else if(caps == 1)
        caps = 0;
      return;
  }

  char ascii = translate(scancode);
  if(ascii > 0){
    buf_char = ascii;
    //putc(ascii);
  }
  last_scancode = scancode;
}

char keyboard_read(){
  while(irq_done!=1){
    asm volatile("sti;hlt;cli");
  }
  irq_done = 0;
  //putc(buf_char);
  return buf_char;
}

void keyboard_install(void){
  irq_install_handler(1, keyboard_handler);
  info("Keyboard Installed");
}