#ifndef _UTILS_H
#define _UTILS_H

#include "ktype.h"

typedef struct registers {
	u16 ds;
	u64 rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
	u32 int_no, err_code;
	u64 rip, eflags, useresp, ss;
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
