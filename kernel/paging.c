#include "paging.h"
#include "mem.h"
#include "ktype.h"
#include "utils.h"

page_directory_t *kernel_directory = 0;
page_directory_t *current_directory = 0;

u32 *frames;
u32 nframes;

extern u32 placement_addr;

#define INDEX_FROM_BIT(a) (a / (8 * 4))
#define OFFSET_FROM_BIT(a) (a % (8 * 4))

void set_bit(u32 frame_addr) {
	u32 frame = frame_addr / 0x1000;
	u32 idx = INDEX_FROM_BIT(frame);
	u32 off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

void clear_bit(u32 frame_addr) {
	u32 frame = frame_addr / 0x1000;
	u32 idx = INDEX_FROM_BIT(frame);
	u32 off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

u32 is_set(u32 frame_addr) {
	u32 frame = frame_addr / 0x1000;
	u32 idx = INDEX_FROM_BIT(frame);
	u32 off = OFFSET_FROM_BIT(frame);
	return (frames[idx] & (0x1 << off));
}

u32 first_frame() {
	u32 i, j;
	for (i = 0; i < INDEX_FROM_BIT(nframes); i++) {
		if (frames[i] != 0xFFFFFFFF) {
			for (j = 0; j < 32; j++) {
				u32 bit = 0x1 << j;
				if (!(frames[i] & bit)) {
					return ((i * 4) * 8) + j;
				}
			}
		}
	}
}

void alloc_frame(page_t *page, i32 is_kernel, i32 is_writeable) {
	if (page->frame != 0) {
		return;	// Frame already allocated
	} else {
		u32 idx = first_frame();
		dentry("idx: ", idx);

		if (idx == (u32)-1) {
			dprint("[PANIC] No free frames!\n");
			for (;;) { asm ("hlt"); }
		}
		set_bit(idx * 0x1000);
		page->present = 1;
		page->rw = (is_writeable) ? 1 : 0;
		page->user = (is_kernel) ? 0 : 1;
		page->frame = idx;
	}
}

void free_frame(page_t *page) {
	u32 frame;
	if (!(frame = page->frame)) {
		return; // Page didn't have any allocated frames!
	} else {
		clear_bit(frame); // free
		page->frame = 0x0;  // remove frame
	}
}

void init_paging() {
	u32 mem_end_page = 0x1000000;

	nframes = mem_end_page / 0x1000;
	frames = (u32 *)kmalloc(INDEX_FROM_BIT(nframes));
	memset(frames, 0, INDEX_FROM_BIT(nframes));

	kernel_directory = (page_directory_t *)kmalloc_a(sizeof(page_directory_t));
	current_directory = kernel_directory;

	i32 i = 0;
	while (i < placement_addr) {
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
		dentry("i: ", i);

	}

	register_interrupt_handler(14, page_fault);

	switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t *dir) {
	current_directory = dir;
	asm volatile("mov %0, %%cr3" :: "r"(&dir->tables_physical));
	u32 cr0;
	asm volatile("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging
	asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

page_t *get_page(u32 addr, i32 make, page_directory_t *dir) {
	addr /= 0x1000;

	u32 table_idx = addr / 1024;
	if (dir->tables[table_idx]) {
		return &dir->tables[table_idx]->pages[addr % 1024];
	} else if (make) {
		u32 tmp;
		dir->tables[table_idx] = (page_table_t *)kmalloc_ap(sizeof(page_table_t), &tmp);
		dir->tables_physical[table_idx] = tmp | 0x7; // Present, Read-Write, User.
		return &dir->tables[table_idx]->pages[addr % 1024];
	} else {
		return 0;
	}
}

void page_fault(registers_t regs) {
	u32 fault_addr;
	asm volatile("mov %%cr2, %0" : "=r" (fault_addr));

	i32 present = !(regs.err_code & 0x1); // Present?
	i32 rw = regs.err_code & 0x2;		  // Writeable?
	i32 us = regs.err_code & 0x4;		  // User mode?
	i32 reserved = regs.err_code & 0x8;	  // Scribbled on?
	i32 id = regs.err_code & 0x10;		  // Bad inst fetch?

	dprint("Page fault! (");
	if (present) { dprint("present "); }
	if (rw) { dprint("read-only "); }
	if (us) { dprint("user-mode "); }
	if (reserved) { dprint("reserved "); }
	dentry(") at 0d", fault_addr);
	dprint("Page fault!\n");
	for (;;) { asm ("hlt"); }
}
