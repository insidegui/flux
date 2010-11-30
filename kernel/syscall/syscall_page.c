/*
 * Copyright (C) 2009, 2010 Nick Johnson <nickbjohnson4224 at gmail.com>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <interrupt.h>
#include <thread.h>
#include <space.h>

/*****************************************************************************
 * syscall_page (int 0x46)
 *
 * EBX: address
 * ECX: count
 * EDX: perm
 * ESI: source
 * EDI: offset
 *
 * Maps memory from the source <source> to the memory region from <address> to
 * <address> + <count> * PAGESZ, ensuring that the memory has at leas the
 * permissions <perm>. Depending on the source, <offset> may be relevant. 
 * Returns zero on success, nonzero on error.
 *
 * Possibilities for <source>:
 *
 * PAGE_NULL - 0
 *     Map null frames to the region, freeing existing frames.
 *
 * PAGE_ANON - 1
 *     Map anonymous memory to the region. This memory has undefined contents,
 *     but is guaranteed not to be in use by any other processes.
 *
 * PAGE_PACK - 2
 *     Map the contents of the current thread's packet (if it exists) to the
 *     region. If the packet is larger than the region, only part of it is
 *     mapped. If the packet is smaller than the region, part of the region is
 *     left empty. Either way, the packet is then freed and becomes 
 *     inaccessible.
 *
 * PAGE_PHYS - 3
 *     Map the physical memory region from <offset> to <offset> + <count> *
 *     PAGESZ to the virtual memory region. This is a privileged function.
 *
 * PAGE_SELF - 4
 *     Map the virtual memory region from <offset> to <offset> + <count> *
 *     PAGESZ to the requested virtual memory region. This function is not yet
 *     implemented.
 *
 * Flags for <perm>:
 *
 * PROT_READ  - 1
 *     Allow page to be read.
 *
 * PROT_WRITE - 2
 *     Allow page to be written.
 *
 * PROT_EXEC  - 4
 *     Allow execution from the page.
 */

struct thread *syscall_page(struct thread *image) {
	uintptr_t address;
	uintptr_t count;
	uintptr_t perm;
	uintptr_t source;
	uintptr_t offset;
	uintptr_t i;

	address = image->ebx;
	count   = image->ecx;
	perm    = image->edx;
	source  = image->esi;
	offset  = image->edi;

	/* check for alignment errors */
	if (address & 0xFFF) {
		image->eax = 1;
		return image;
	}

	/* check bounds of region */
	if (address >= KSPACE || address + (count * PAGESZ) >= KSPACE) {
		image->eax = 1;
		return image;
	}

	/* reject insane requests (> 64MB) */
	if (count > 0x4000) {
		image->eax = 1;
		return image;
	}

	/* change <perm> to real page flags */
	perm = PF_USER | PF_PRES | ((perm & 2) ? PF_RW : 0);

	switch (source) {
	case 0: /* PAGE_NULL */
		
		/* free all allocated pages in the region, set the frames to zero */
		for (i = address; i < address + count * PAGESZ; i += PAGESZ) {
			if (page_get(i) & PF_PRES) {
				frame_free(page_ufmt(page_get(i)));
				page_set(i, 0);
			}
		}

		break;
	case 1: /* PAGE_ANON */

		/* allocate requested pages, but don't free old ones if they're there */
		for (i = address; i < address + count * PAGESZ; i += PAGESZ) {
			if ((page_get(i) & PF_PRES) == 0) {
				page_set(i, page_fmt(frame_new(), perm));
			}
		}

		break;
	case 2: /* PAGE_PACK */

		/* reject if there is no packet */
		if (!image->packet || !image->packet->frame) {
			image->eax = 1;
			return image;
		}

		/* 
		 * NOTE - right now, packets are only one frame, 
		 * but they will be larger later!
		 */

		/* skip if the request is too small */
		if (count == 0) {
			image->eax = 0;
			return image;
		}

		/* free any encumbering frame */
		if (page_get(address) & PF_PRES) {
			frame_free(page_get(address));
		}

		/* map the packet */
		page_set(address, page_fmt(image->packet->frame, perm));

		break;
	case 3: /* PAGE_PHYS */
		
		/* check effective permissions */
		if (image->user) {
			image->eax = 1;
			return image;
		}

		/* allocate requested frames */
		for (i = 0; i < count * PAGESZ; i += PAGESZ) {

			/* free encumbering frames */
			if (page_get(address + i) & PF_PRES) {
				frame_free(page_ufmt(page_get(address + i)));
			}

			page_set(address + i, page_fmt(offset + i, perm));
		}

		break;
	default:
		image->eax = 1;
		return image;
	}

	image->eax = 0;
	return image;
}