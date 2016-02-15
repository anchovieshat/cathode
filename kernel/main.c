#include "utils.h"
#include "vga.h"
#include "serial.h"

void make_line() {
  dputc('+');
  for (u32 i = 0; i < VGA_WIDTH - 2; i++) {
    dputc('-');
  }
  dputc('+');
}

void center_message(char *msg) {
  u32 msg_len = strlen(msg);
  u32 center_msg = msg_len / 2;
  u32 str_itr = 0;
  u8 tack_on = 0;

  if ((msg_len % 2) == 0) {
    tack_on = 1;
  } else {
    tack_on = 2;
  }

  dputc('|');
  for(u32 i = 1; i < VGA_WIDTH - 1; i++) {
    if (i > (VGA_WIDTH / 2) - center_msg && i < ((VGA_WIDTH / 2) + center_msg) + tack_on) {
      dputc(msg[str_itr]);
      str_itr++;
    } else {
      dputc(' ');
    }
  }
  dputc('|');
}

void _kmain() {
  init_serial();
  init_term();

  make_line();
  center_message("Welcome to Cathode!");
  make_line();
}
