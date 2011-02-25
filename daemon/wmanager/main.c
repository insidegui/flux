/*
 * Copyright (C) 2011 Jaagup Repan
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

#include "wmanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <driver.h>
#include <exec.h>
#include <proc.h>
#include <page.h>
#include "list.h"

const uint32_t bitmaps_dir = 1, windows_dir = 2;
uint64_t vgafd;
uint8_t *screen;
size_t screen_width, screen_height;

void forward_message(const char *msg) {
	if (active_window) {
		struct vfs_obj *window_file = vfs_get_index(active_window->id);
		uint32_t pid;
		sscanf(window_file->name, "%i", &pid);
		rcall(RP_CONS(pid, 0), msg);
	}
}

char *wmanager_rcall(uint64_t source, struct vfs_obj *file, const char *args) {
	if (strlen(args) <= 1) {
		return NULL;
	}

	if (args[0] == 'm') { // mouse move
		int dx, dy;
		if (sscanf(args + 2, "%i %i", &dx, &dy) == 2) {
			mouse_move(dx, dy);
			forward_message(args);
		}
		return NULL; // mouse driver doesn't check return value
	}

	if (args[0] == 'c') { // mouse click
		int buttons;
		if (sscanf(args + 2, "%i", &buttons) == 1) {
			mouse_click(buttons);
			forward_message(args);
		}
		return NULL;
	}

	if (args[0] == 'r') { // mouse release
		int buttons;
		if (sscanf(args + 2, "%i", &buttons) == 1) {
			mouse_release(buttons);
			forward_message(args);
		}
		return NULL;
	}

	if (args[0] == 's') { // size
		size_t width, height;
		if (sscanf(args + 2, "%i %i", &width, &height) != 2) {
			return NULL;
		}
		if (set_window_size(file->index, width, height) != 0) {
			return NULL;
		}
	}

	else  if (args[0] == 'b') { //bitmap
		uint32_t bitmap;
		if (sscanf(args + 2, "%i", &bitmap) != 1) {
			return NULL;
		}
		if (set_window_bitmap(file->index, bitmap) != 0) {
			return NULL;
		}
	}

	else {
		return NULL;
	}

	return strdup("ok");
}

int wmanager_share(uint64_t source, struct vfs_obj *file, uint8_t *buffer, size_t size, uint64_t off) {
	if (off != 0) {
		return -1;
	}
	return set_bitmap(file->index, buffer, size);
}

int wmanager_sync(uint64_t source, struct vfs_obj *file) {
	memset(screen, 0, screen_width * screen_height * 3);
	struct window_t *window = windows;
	while (window) {
		draw_window(window);
		window = window->next;
	}
	draw_cursor();
	sync(vgafd);
	return 0;
}

struct vfs_obj *wmanager_cons(int type) {
	static int next_index = 1;
	struct vfs_obj *fobj = NULL;

	switch (type) {
	case RP_TYPE_FILE:
	case RP_TYPE_DIR:
		fobj        = calloc(sizeof(struct vfs_obj), 1);
		fobj->type  = type;
		fobj->size  = 0;
		fobj->link  = 0;
		fobj->data  = NULL;
		fobj->index = next_index++;
		fobj->acl   = acl_set_default(fobj->acl, PERM_READ | PERM_WRITE);
		break;
	}
	
	return fobj;
}

int wmanager_push(struct vfs_obj *file) {
	if (file->index == bitmaps_dir || file->index == windows_dir) {
		return 0;
	}

	if (file->mother->index == windows_dir) {
		return add_window(file->index);
	}

	if (file->mother->index == bitmaps_dir) {
		return add_bitmap(file->index);
	}

	return -1;
}

int wmanager_pull(struct vfs_obj *file) {
	if (file->mother->index == windows_dir) {
		return remove_window(file->index);
	}

	else if (file->mother->index == bitmaps_dir) {
		return remove_bitmap(file->index);
	}

	return -1;
}

//todo: owner control
int main(int argc, char **argv) {
	struct vfs_obj *root;
	char buffer[32];

	stdout = stderr = fopen("/dev/serial", "w");

	if (fork() < 0) {
		exec("/sbin/vga");
	}
	mwait(PORT_CHILD, 0);

	root        = calloc(sizeof(struct vfs_obj), 1);
	root->type  = RP_TYPE_DIR;
	root->size  = 0;
	root->acl   = acl_set_default(root->acl, PERM_READ | PERM_WRITE);
	vfs_set_index(0, root);

	di_wrap_rcall(wmanager_rcall);
	di_wrap_share(wmanager_share);
	di_wrap_sync (wmanager_sync);
	vfs_wrap_cons(wmanager_cons);
	vfs_wrap_push(wmanager_push);
	vfs_wrap_pull(wmanager_pull);
	vfs_wrap_init();

	io_link("/sys/wmanager", RP_CONS(getpid(), 0));
	io_cons("/sys/wmanager/bitmaps", RP_TYPE_DIR);
	io_cons("/sys/wmanager/windows", RP_TYPE_DIR);

	vgafd = io_find("/dev/vga0");
	sscanf(rcall(vgafd, "dim"), "%i %i", &screen_width, &screen_height);
	screen = malloc(screen_width * screen_height * 3);
	memset(screen, 0, screen_width * screen_height * 3);
	share(vgafd, screen, screen_width * screen_height * 3, 0, PROT_READ);

	sprintf(buffer, "%u", getpid());
	rcall(io_find("/dev/mouse"), buffer);

	if (fork() < 0) {
		exec("/bin/testapp");
	}

	_done();

	free(screen);
	LIST_FREE(bitmap)
	LIST_FREE(window)
	return 0;
}
