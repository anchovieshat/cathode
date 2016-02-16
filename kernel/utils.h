#ifndef _UTILS_H
#define _UTILS_H

#include "ktype.h"

void outb(u16 port, u8 value);
u8 inb(u16 port);
void dputc(char c);
void dprint(char *str);
void dputn(u32 n);
void dentry(char *str, u32 n);
u32 strlen(char *str);

#endif
