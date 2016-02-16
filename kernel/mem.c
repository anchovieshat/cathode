#include "mem.h"
#include "ktype.h"

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
