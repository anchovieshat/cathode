#ifndef _UTILS_H
#define _UTILS_H

#include "ktype.h"

typedef struct registers
{
   u32 ds;
   u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pushad.
   u32 int_no, err_code;    // Interrupt number and error code (if applicable)
   u32 eip, cs, eflags, ss; // Pushed by the processor automatically.
} registers_t;

void outb(u16 port, u8 value);
void outw(u16 port, u16 value);
u8 inb(u16 port);
void io_wait();
void reboot();
void dputc(char c);
void dprint(char *str);
void dputn(u32 n);
void dentry(char *str, u32 n);
void print_regs(registers_t regs);
u32 strlen(char *str);

#endif
