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
	dentry("ds: ", regs.ds);
	dentry("edi: ", regs.edi);
	dentry("esi: ", regs.esi);
	dentry("ebp: ", regs.ebp);
	dentry("ebx: ", regs.ebx);
	dentry("edx: ", regs.edx);
	dentry("ecx: ", regs.ecx);
	dentry("eax: ", regs.eax);
	dentry("int_no: ", regs.int_no);
	dentry("err_code: ", regs.err_code);
	dentry("cs: ", regs.cs);
	dentry("eflags: ", regs.eflags);
	dentry("esp: ", regs.esp);
	dentry("ss: ", regs.ss);
}

u32 strlen(char *str) {
	u32 len = 0;
	while (str[len] != 0) { len++; }
	return len;
}
