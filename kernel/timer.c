#include "timer.h"
#include "ktype.h"
#include "utils.h"
#include "interrupt.h"

u32 tick = 0;

void timer_callback(registers_t regs) {
	tick++;
	dentry("Tick: ", tick);
}

void init_timer(u32 freq) {
	register_interrupt_handler(IRQ0, &timer_callback);

	// Div must be below 16 bits.
	u32 div = 1193180 / freq;

	// Send timer command byte
	outb(0x43, 0x36);

	u8 low = (u8)(div & 0xFF);
	u8 high = (u8)((div >> 8) & 0xFF);

	// Send freq divisor
	outb(0x40, low);
	outb(0x40, high);
}
