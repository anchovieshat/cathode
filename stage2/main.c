typedef unsigned short u16;

struct ctx {
	u16 ds, es;
	u16 ax;
	u16 bx;
	u16 cx;
	u16 dx;
	u16 si;
	u16 di;
	u16 bp;
	u16 flags;
};

extern int intcall(char num, struct ctx ctx); // yes, pass it on the stack
void puts(const char *s);
void putc(char ch);

unsigned long strlen(const char *s) {
	int l = 0;
	while(*s) ++l;
	return l;
}

void s2main(void) {
	puts("Hello from 32-bit C");
}

void puts(const char *s) {
	while (*s)
		putc(*s++);
	putc('\r');
	putc('\n');
}

void putc(char ch) {
	struct ctx c;
	c.ax = (0x0E << 8) | ch;
	c.bx = 0;
	c.cx = 0;
	c.dx = 0;
	c.si = 0;
	c.di = 0;
	c.bp = 0;
	c.flags = 0;
	c.ds = 0;
	c.es = 0;
	intcall(0x10, c);
}
