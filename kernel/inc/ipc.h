/*
 * Copyright (C) 2009, 2010 Nick Johnson <nickbjohnson4224 at gmail.com>
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

#ifndef KERNEL_IPC_H
#define KERNEL_IPC_H

#include <process.h>
#include <stdint.h>
#include <thread.h>
#include <types.h>

#define PORT_QUIT	0
#define PORT_TERM	1
#define PORT_ABORT	2
#define PORT_KILL	3
#define PORT_STOP	4
#define PORT_CONT	5
#define PORT_TRAP	6
#define PORT_INT	7
#define PORT_IRQ	8
#define PORT_ALARM	9
#define PORT_CHILD	10
#define PORT_FLOAT	11
#define PORT_PAGE	12
#define PORT_ILL	13
#define PORT_USER1	14
#define PORT_USER2	15

struct msg {
	size_t    count;
	uint32_t *frame;	
};

#endif/*KERNEL_IPC_H*/
