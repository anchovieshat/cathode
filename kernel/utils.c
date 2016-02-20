#include "utils.h"
#include "ktype.h"
#include "serial.h"
#include "vga.h"

void outb(u16 port, u8 value) {
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

void outw(u16 port, u16 value) {
	asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

u8 inb(u16 port) {
	u8 rv;
	asm volatile ("inb %1, %0" : "=a" (rv) : "dN" (port));
	return rv;
}

void io_wait() {
	asm volatile ( "outb %%al, $0x80" : : "a"(0) );
}

void dprint(char *str) {
	vprint(str);
	sprint(str);
}

void dputc(char c) {
	vputc(c);
	sputc(c);
}

void dputn(u32 n) {
	vputn(n);
	sputn(n);
}

void dentry(char *str, u32 n) {
	dprint(str);
	dputn(n);
	dputc('\n');
}

void print_regs(registers_t regs) {
	dentry("rdi: ", regs.rdi);
	dentry("rsi: ", regs.rsi);
	dentry("rbp: ", regs.rbp);
	dentry("rsp: ", regs.rsp);
	dentry("rbx: ", regs.rbx);
	dentry("rdx: ", regs.rdx);
	dentry("rcx: ", regs.rcx);
	dentry("rax: ", regs.rax);
	dentry("int_no: ", regs.int_no);
	dentry("err_code: ", regs.err_code);
	dentry("rip: ", regs.rip);
	dentry("eflags: ", regs.eflags);
	dentry("useresp: ", regs.useresp);
}

u32 strlen(char *str) {
	u32 len = 0;
	while (str[len] != 0) { len++; }
	return len;
}

void reboot() {
	asm volatile ("cli");

	u8 tmp = 0x02;
	while (tmp & 0x02)
		tmp = inb(0x64);
	outb(0x64, 0xFE);
}
