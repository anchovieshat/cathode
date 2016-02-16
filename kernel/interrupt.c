#include "interrupt.h"
#include "utils.h"

isr_t interrupt_handlers[256];

void interrupt_handler(registers_t regs) {
	if (regs.int_no > 31) {
		irq_handler(regs);
	} else {
		isr_handler(regs);
	}
}

void isr_handler(registers_t regs) {
}

void irq_handler(registers_t regs) {

	if (regs.int_no >= 40) {
		outb(0xA0, 0x20); // Send reset signal to slave PIC.
	}

	outb(0x20, 0x20); // Send reset signal to master PIC. (As well as slave PIC, if necessary).
	io_wait();

	if (interrupt_handlers[regs.int_no] != 0) {
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}

void register_interrupt_handler(u8 n, isr_t handler) {
	interrupt_handlers[n] = handler;
}
