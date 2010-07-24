/*
 * Copyright 2010 Nick Johnson
 * ISC Licensed, see LICENSE for details
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <arch.h>
#include <ipc.h>

size_t ssend(FILE *fd, char *r, char *s, size_t size, uint64_t off, uint8_t port) {
	struct packet *p_out;
	struct packet *p_in;
	size_t datasize, oldsize, frag;
	size_t spos, rpos;
	event_t old_handler;

	old_handler = when(PORT_REPLY, NULL);
	oldsize     = size;
	p_out       = packet_alloc(0);
	p_in        = NULL;
	frag        = 0;
	spos        = 0;
	rpos        = 0;

	while (size) {
		datasize = (size > PACKET_MAXDATA) ? PACKET_MAXDATA : size;

		packet_setbuf(&p_out, datasize);

		p_out->identity = 0;
		p_out->protocol = PACKET_PROTOCOL;

		p_out->fragment_index = frag;
		p_out->fragment_count = ((size - 1) / PACKET_MAXDATA + 1);
		p_out->target_pid     = fd->server;
		p_out->target_inode   = fd->inode;
		p_out->offset         = off;

		if (s) {
			memcpy(packet_getbuf(p_out), &s[spos], datasize);
			spos += datasize;
		}

		send(port, fd->server, p_out);
		p_in = waits(PORT_REPLY, fd->server);

		if (!p_in || p_in->data_length == 0) {
			if (p_in) packet_free(p_in);
			break;
		}

		if (r) {
			memcpy(&r[rpos], packet_getbuf(p_in), p_in->data_length);
			rpos += p_in->data_length;
		}

		size -= p_in->data_length;
		off  += p_in->data_length;

		packet_free(p_in);

		frag++;
	}

	packet_free(p_out);
	when(PORT_REPLY, old_handler);

	return (oldsize - size);
}