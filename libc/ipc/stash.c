/*
 * Copyright 2010 Nick Johnson
 * ISC Licensed, see LICENSE for details
 */

#include <stdlib.h>
#include <mutex.h>
#include <ipc.h>

/****************************************************************************
 * stash
 *
 * Queues a message based on its metadata.
 */

void stash(struct packet *packet, uint8_t port, uint32_t source) {
	struct message *m;

	m = malloc(sizeof(struct message));
	m->packet = packet;
	m->source = source;

	mutex_spin(&m_msg_queue[port]);

	m->prev = &msg_queue[port];
	m->next =  msg_queue[port].next;
	msg_queue[port].next = m;

	mutex_free(&m_msg_queue[port]);
}