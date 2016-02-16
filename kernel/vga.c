#include "vga.h"
#include "utils.h"

u32 term_row;
u32 term_col;
u16 term_color;
u16 *term_buf;
u32 cur_y;
u32 cur_x;

static inline u16 make_color(vga_color fg, vga_color bg) {
	return ((bg << 4) | (fg & 0x0F)) << 8;
}

static inline u16 make_vgaentry(char c, u16 color) {
	return c | color;
}

void init_term() {
	term_row = 0;
	term_col = 0;
	term_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	term_buf = VGA_MEMORY;
	clear_term();
}

void clear_term() {
	for (u32 y = 0; y < VGA_HEIGHT; y++) {
		for (u32 x = 0; x < VGA_WIDTH; x++) {
			const u32 i = (y * VGA_WIDTH) + x;
			term_buf[i] = make_vgaentry(' ', term_color);
		}
	}
}

void move_cursor() {
	u16 cur_pos = (cur_y * VGA_WIDTH) + cur_x;
	outb(0x3D4, 14); // init high send
	outb(0x3D5, cur_pos >> 8); // send high cursor byte
	outb(0x3D4, 15); // init low send
	outb(0x3D5, cur_pos); // send low cursor byte
}

void scroll() {
	if (cur_y >= VGA_HEIGHT) {
		u32 i;

		// Scroll everything up a line
		for (i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
			VGA_MEMORY[i] = VGA_MEMORY[i+80];
		}

		// Clear the bottom line
		for (i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < (VGA_HEIGHT * VGA_WIDTH); i++) {
			VGA_MEMORY[i] = make_vgaentry(' ', term_color);
		}

		// Move the cursor to the last line
		cur_y = VGA_HEIGHT - 1;
	}
}

void vputc(char c) {
	// is a displayable character?
	if (c >= ' ') {
		const u32 i = (cur_y * VGA_WIDTH) + cur_x;
		term_buf[i] = make_vgaentry(c, term_color);
		cur_x++;
	} else if (c == '\n') {
		cur_x = 0;
		cur_y++;
	}

	// new line?
	if (cur_x >= 80) {
		cur_x = 0;
		cur_y++;
	}

	scroll();
	move_cursor();
}

void vprint(char *str) {
	u32 len = 0;
	while (str[len] != 0) {
		vputc(str[len]);
		len++;
	}
}

void vputn(u32 n) {
	if (n == 0) {
		vputc('0');
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
	vprint(c2);
}
