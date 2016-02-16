#ifndef _GDT_H
#define _GDT_H

#include "ktype.h"

#define GDT_ENTRIES 3

struct gdt_entry {
	u16 limit_low;
	u16 base_low;
	u8 base_middle;
	u8 access;
	u8 base_high;
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

struct gdt_ptr {
	u16 limit;
	u32 base;
} __attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

extern void gdt_flush();
void print_gdtr();
void print_gdt_entry();

#endif
