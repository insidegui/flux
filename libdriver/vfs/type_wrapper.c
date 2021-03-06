/*
 * Copyright (C) 2009-2011 Nick Johnson <nickbjohnson4224 at gmail.com>
 * 
 * Permission to use, copy, modify, and distribute this software for any
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

#include <driver.h>
#include <stdlib.h>
#include <mutex.h>
#include <natio.h>
#include <proc.h>

/*****************************************************************************
 * __type_wrapper
 *
 * Performs the requested actions of a FS_TYPE command.
 *
 * protocol:
 *   port: PORT_TYPE
 *
 *   request:
 *
 *   reply:
 *     uint8_t type
 */

void __type_wrapper(struct msg *msg) {
	struct vfs_obj *file;

	/* find file node */
	file = vfs_get_index(RP_INDEX(msg->target));

	if (!file) {
		merror(msg);
		return;
	}

	msg->length = 1;
	msg->data[0] = file->type;
	mreply(msg);
}
