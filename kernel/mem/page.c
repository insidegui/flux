// Copyright 2009 Nick Johnson

#include <lib.h>
#include <mem.h>

pool_t *fpool;

void mem_alloc(uintptr_t base, uintptr_t size, uint16_t flags) {
	uint32_t i;

	i = base & ~0xFFF;
	while (i < (base + size)) {
		if ((page_get(i) & PF_PRES) == 0) {
			p_alloc(i, flags);
		}
		i += 0x1000;
	}
}

void mem_free(uintptr_t base, uintptr_t size) {
	uint32_t i;

	printk("Freeing %x size %x\n", base, size);

	i = base & ~0xFFF;
	while (i < (base + size)) {
		if (page_get(i) & PF_PRES) {
			p_free(i);
		}
		i += 0x1000;
	}
}

void page_touch(uint32_t page) {
	page &= ~0x3FFFFF;
	if (cmap[page >> 22] & PF_PRES) return;
	cmap[page >> 22] = frame_new() | (PF_PRES | PF_RW | PF_USER);
	pgclr(&ctbl[page >> 12]);
}

void page_set(uint32_t page, page_t value) {
	page &= ~0xFFF;
	if ((cmap[page >> 22] & PF_PRES) == 0) page_touch(page);
	ctbl[page >> 12] = value;
//	page_flush(page);
	asm volatile ("invlpg %0" :: "m" (page));
}

page_t page_get(uint32_t page) {
	return (cmap[page >> 22] & PF_PRES) ? ctbl[page >> 12] : 0;
}

void temp_touch(uint32_t page) {
	page &= ~0x3FFFFF;
	uint32_t target = (uint32_t) &ttbl[page >> 12];
	if (tmap[page >> 22] & PF_PRES) return;
	tmap[page >> 22] = frame_new() | (PF_PRES | PF_RW | PF_USER);
//	page_flush((uint32_t) &ttbl[page >> 12]);
	asm volatile ("invlpg %0" :: "m" (target));
	pgclr(&ctbl[page >> 12]);
}

void temp_set(uint32_t page, page_t value) {
	page &= ~0xFFF;
	if ((tmap[page >> 22] & PF_PRES) == 0) temp_touch(page);
	ttbl[page >> 12] = value;
//	page_flush(page);
	asm volatile ("invlpg %0" :: "m" (page));
}

page_t temp_get(uint32_t page) {
	return (tmap[page >> 22] & PF_PRES) ? ttbl[page >> 12] : 0;
}
