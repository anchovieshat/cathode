#include "kb.h"
#include "utils.h"
#include "ktype.h"
#include "interrupt.h"

void keyboard_callback(registers_t regs) {
	u8 scancode;
	scancode = inb(0x60);

	if (scancode == 29) {
		kb_us[scancode] = 1;
	} else if (scancode == 157) {
		kb_us[29] = 0;
	} else if (scancode == 42) {
		kb_us[42] = 1;
	} else if (scancode == 170) {
		kb_us[42] = 0;
	}

	if (scancode & 0x80) {
		//dentry("special key: ", scancode);
	}

	if ((kb_us[scancode] == 'c') && (kb_us[29] == 1)) {
		dprint("^c");
	} else if (scancode < 128) {
		if (kb_us[42]) {
			dputc(kb_us[scancode] - 32);
		} else {
			dputc(kb_us[scancode]);
		}
	}
}

void init_keyboard() {
	register_interrupt_handler(IRQ1, &keyboard_callback);
}

// BIOS reboot
// outb(0x0, 0x70)
// outb(0x8f, 0x71)
