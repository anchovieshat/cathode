#include "mem.h"
#include "ktype.h"

extern u32 end;
u32 placement_addr = (u32)&end;

u8 *memcpy(u8 *dest, const u8 *src, i32 count) {
	for (i32 i = 0; i < count; i++) {
		dest[i] = src[i];
	}

	return dest;
}

u8 *memset(u8 *dest, u8 val, i32 count) {
	for (i32 i = 0; i < count; i++) {
		dest[i] = val;
	}
	return dest;
}

u32 kmalloc_internal(u32 size, i32 align, u32 *phys_addr) {
	if (align == 1 && (placement_addr & 0xFFFFF000)) {
		placement_addr &= 0xFFFFF000;
		placement_addr += 0x1000;
	}
	if (phys_addr) {
		*phys_addr = placement_addr;
	}
	u32 tmp = placement_addr;
	placement_addr += size;
	return tmp;
}

u32 kmalloc_a(u32 size) {
	return kmalloc_internal(size, 1, 0);
}

u32 kmalloc_p(u32 size, u32 *phys_addr) {
	return kmalloc_internal(size, 0, phys_addr);
}

u32 kmalloc_ap(u32 size, u32 *phys_addr) {
	return kmalloc_internal(size, 1, phys_addr);
}

u32 kmalloc(u32 size) {
	return kmalloc_internal(size, 0, 0);
}
