#include "gdt.h"
#include "utils.h"

extern gdt_entry_t gdt64[GDT_ENTRIES];
extern gdt_ptr_t gdtr64;

void print_gdtr() {
	dprint("GDTR\n");
	dentry("limit: ", gdtr64.limit);
	dentry("base: ", gdtr64.base);
}

void print_gdt_entry(u32 i) {
	dentry("GDT entry ", i);
	dentry("Limit low: ", gdt64[i].limit_low);
	dentry("Base low: ", gdt64[i].base_low);
	dentry("Base middle: ", gdt64[i].base_middle);
	dentry("Access: ", gdt64[i].access);
	dentry("Base high: ", gdt64[i].base_high);
}
