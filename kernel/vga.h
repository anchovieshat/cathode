#ifndef _KERNEL_VGA_H
#define _KERNEL_VGA_H

#include "ktype.h"

#define VGA_HEIGHT 25
#define VGA_WIDTH 80

static u16 *const VGA_MEMORY = (u16 *)0xB8000;

typedef enum vga_colors {
  COLOR_BLACK,
  COLOR_BLUE,
  COLOR_GREEN,
  COLOR_CYAN,
  COLOR_RED,
  COLOR_MAGENTA,
  COLOR_BROWN,
  COLOR_LIGHT_GREY,
  COLOR_DARK_GREY,
  COLOR_LIGHT_BLUE,
  COLOR_LIGHT_GREEN,
  COLOR_LIGHT_CYAN,
  COLOR_LIGHT_RED,
  COLOR_LIGHT_MAGENTA,
  COLOR_LIGHT_BROWN,
  COLOR_WHITE,
} vga_color;

void init_term();
void vputc(char c);
void vprint(char *c);
void vputn(u32 n);
void clear_term();

#endif
