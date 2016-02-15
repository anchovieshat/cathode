#ifndef _SERIAL_H
#define _SERIAL_H

#include "ktype.h"

#define PORT 0x3f8

i32 is_transmit_empty();
void init_serial();
void sputc(char a);
void sprint(char *str);

#endif
