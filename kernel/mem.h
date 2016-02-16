#ifndef _MEM_H
#define _MEM_H

#include "ktype.h"

u8 *memcpy(u8 *dest, const u8 *src, i32 count);
u8 *memset(u8 *dest, u8 val, i32 count);
u32 kmalloc_internal(u32 size, i32 align, u32 *phys_addr);
u32 kmalloc_a(u32 size);
u32 kmalloc_p(u32 size, u32 *phys_addr);
u32 kmalloc_ap(u32 size, u32 *phys_addr);
u32 kmalloc(u32 size);

#endif
