#ifndef PAGING_H
#define PAGING_H

#include "ktype.h"
#include "interrupt.h"
#include "utils.h"

typedef struct page {
	u32 present		: 1;	// Present in memory?
	u32 rw			: 1;	// 1 - Read-only | 0 - Read-Write
	u32 user		: 1;	// 0 - Supervisor level
	u32 accessed	: 1;	// Accessed since last refresh?
	u32 dirty		: 1;	// Written to since last refresh?
	u32 unused		: 7;	// Unused/Reserved
	u32 frame		: 20;	// Frame address << 12
} page_t;

typedef struct page_table {
	page_t pages[1024];
} page_table_t;

typedef struct page_directory {
	page_table_t *tables[1024];
	u32 tables_physical[1024];
	u32 phys_addr;
} page_directory_t;

void init_paging();

void alloc_frame(page_t *page, int is_kernel, int is_writeable);


void switch_page_directory(page_directory_t *new);
page_t *get_page(u32 addr, i32 make, page_directory_t *dir);
void page_fault(registers_t reg);

#endif
