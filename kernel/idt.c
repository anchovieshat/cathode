#include "idt.h"
#include "ktype.h"
#include "mem.h"
#include "utils.h"

idt_entry_t idt[256];
idt_ptr_t idtp;

extern void idt_load();

void idt_set_gate(u8 num, u64 base, u16 sel, u8 flags) {
   idt[num].base_low  = base & 0xFFFF;
   idt[num].base_mid  = (base >> 16) & 0xFFFF;
   idt[num].base_high =	(base >> 32) & 0xFFFFFFFF;
   idt[num].selector  = sel;
   idt[num].reserved  = 0;
   idt[num].reserved2 = 0;
   idt[num].flags     = flags;
}

void init_idt() {

	idtp.limit = (sizeof(idt_entry_t) * 256) - 1;
	idtp.base = (u64)&idt;

	memset((u8 *)&idt, 0, sizeof(idt_entry_t) * 256);

	outb(0x20, 0x11);
	io_wait();
	outb(0xA0, 0x11);
	io_wait();
	outb(0x21, 0x20);
	io_wait();
	outb(0xA1, 0x28);
	io_wait();
	outb(0x21, 0x04);
	io_wait();
	outb(0xA1, 0x02);
	io_wait();
	outb(0x21, 0x01);
	io_wait();
	outb(0xA1, 0x01);
	io_wait();

	outb(0x21, 0x0);
	outb(0xA1, 0x0);

	dprint("Set up PIC\n");

	idt_set_gate(0, (u64)isr0, 0x08, 0x8E);
	idt_set_gate(1, (u64)isr1, 0x08, 0x8E);
	idt_set_gate(2, (u64)isr2, 0x08, 0x8E);
	idt_set_gate(3, (u64)isr3, 0x08, 0x8E);
	idt_set_gate(4, (u64)isr4, 0x08, 0x8E);
	idt_set_gate(5, (u64)isr5, 0x08, 0x8E);
	idt_set_gate(6, (u64)isr6, 0x08, 0x8E);
	idt_set_gate(7, (u64)isr7, 0x08, 0x8E);
	idt_set_gate(8, (u64)isr8, 0x08, 0x8E);
	idt_set_gate(9, (u64)isr9, 0x08, 0x8E);
	idt_set_gate(10, (u64)isr10, 0x08, 0x8E);
	idt_set_gate(11, (u64)isr11, 0x08, 0x8E);
	idt_set_gate(12, (u64)isr12, 0x08, 0x8E);
	idt_set_gate(13, (u64)isr13, 0x08, 0x8E);
	idt_set_gate(14, (u64)isr14, 0x08, 0x8E);
	idt_set_gate(15, (u64)isr15, 0x08, 0x8E);
	idt_set_gate(16, (u64)isr16, 0x08, 0x8E);
	idt_set_gate(17, (u64)isr17, 0x08, 0x8E);
	idt_set_gate(18, (u64)isr18, 0x08, 0x8E);
	idt_set_gate(19, (u64)isr19, 0x08, 0x8E);
	idt_set_gate(20, (u64)isr20, 0x08, 0x8E);
	idt_set_gate(21, (u64)isr21, 0x08, 0x8E);
	idt_set_gate(22, (u64)isr22, 0x08, 0x8E);
	idt_set_gate(23, (u64)isr23, 0x08, 0x8E);
	idt_set_gate(24, (u64)isr24, 0x08, 0x8E);
	idt_set_gate(25, (u64)isr25, 0x08, 0x8E);
	idt_set_gate(26, (u64)isr26, 0x08, 0x8E);
	idt_set_gate(27, (u64)isr27, 0x08, 0x8E);
	idt_set_gate(28, (u64)isr28, 0x08, 0x8E);
	idt_set_gate(29, (u64)isr29, 0x08, 0x8E);
	idt_set_gate(30, (u64)isr30, 0x08, 0x8E);
	idt_set_gate(31, (u64)isr31, 0x08, 0x8E);
	idt_set_gate(32, (u64)irq0, 0x08, 0x8E);
	idt_set_gate(33, (u64)irq1, 0x08, 0x8E);
	idt_set_gate(34, (u64)irq2, 0x08, 0x8E);
	idt_set_gate(35, (u64)irq3, 0x08, 0x8E);
	idt_set_gate(36, (u64)irq4, 0x08, 0x8E);
	idt_set_gate(37, (u64)irq5, 0x08, 0x8E);
	idt_set_gate(38, (u64)irq6, 0x08, 0x8E);
	idt_set_gate(39, (u64)irq7, 0x08, 0x8E);
	idt_set_gate(40, (u64)irq8, 0x08, 0x8E);
	idt_set_gate(41, (u64)irq9, 0x08, 0x8E);
	idt_set_gate(42, (u64)irq10, 0x08, 0x8E);
	idt_set_gate(43, (u64)irq11, 0x08, 0x8E);
	idt_set_gate(44, (u64)irq12, 0x08, 0x8E);
	idt_set_gate(45, (u64)irq13, 0x08, 0x8E);
	idt_set_gate(46, (u64)irq14, 0x08, 0x8E);
	idt_set_gate(47, (u64)irq15, 0x08, 0x8E);

	dprint("Setup Interrupts\n");

	idt_load();
	dprint("Loaded IDT\n");
}
