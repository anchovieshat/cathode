#include "utils.h"
#include "ktype.h"
#include "serial.h"
#include "vga.h"

void outb(u16 port, u8 value) {
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8 inb(u16 port) {
	u8 rv;
	asm volatile ("inb %1, %0" : "=a" (rv) : "dN" (port));
	return rv;
}

void dprint(char *str) {
  vprint(str);
  sprint(str);
}

void dputc(char c) {
  vputc(c);
  sputc(c);
}

u32 strlen(char *str) {
  u32 len = 0;
  while (str[len] != 0) { len++; }
  return len;
}
