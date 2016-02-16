#include "serial.h"
#include "ktype.h"
#include "utils.h"

i32 is_transmit_empty() {
	return inb(PORT + 5) & 0x20;
}

void init_serial() {
	outb(PORT + 1, 0x00); // Disable all interrupts
	outb(PORT + 3, 0x80); // Set baud rate divisor
	outb(PORT + 0, 0x03); // 38400 baud
	outb(PORT + 1, 0x00); //
	outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
	outb(PORT + 2, 0xC7); // Enable FIFO, clear with 14-byte threshold
	outb(PORT + 2, 0x0B); // IRQs enabled, RTS/DSR set
}

void sputc(char a) {
	while (is_transmit_empty() == 0);
	outb(PORT, a);
}

void sputn(u32 n) {
	if (n == 0) {
		sputc('0');
		return;
	}

	char c[32];
	i32 acc = n;
	i32 i = 0;
	while (acc > 0) {
		c[i] = '0' + acc%10;
		acc /= 10;
		i++;
	}
	c[i] = 0;

	char c2[32];
	c2[i--] = 0;
	i32 j = 0;
	while(i >= 0) {
		c2[i--] = c[j++];
	}
	sprint(c2);
}

void sprint(char *str) {
	u32 len = 0;
	while (str[len] != 0) {
		sputc(str[len]);
		len++;
	}
}
