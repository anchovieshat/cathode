#include "gdt.h"
#include "utils.h"

extern gdt_entry_t gdt[GDT_ENTRIES];
extern gdt_ptr_t gdtr;

void print_gdtr() {
	dprint("GDTR\n");
	dentry("limit: ", gdtr.limit);
	dentry("base: ", gdtr.base);
}

void print_gdt_entry(u32 i) {
	dentry("GDT entry ", i);
	dentry("Limit low: ", gdt[i].limit_low);
	dentry("Base low: ", gdt[i].base_low);
	dentry("Base middle: ", gdt[i].base_middle);
	dentry("Access: ", gdt[i].access);
	dentry("Base high: ", gdt[i].base_high);
}
