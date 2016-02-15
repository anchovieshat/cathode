#ifndef _SERIAL_H
#define _SERIAL_H

#define PORT 0x3f8

void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
int is_transmit_empty();
void init_serial();
void sputc(char a);
void sprint(char *str);

#endif
