#include "ringbuffer.h"

void ringbuffer_init(struct ringbuffer* buf) {
	buf->head = 0;
	buf->tail = 0;
}

void fixIndex(size_t* index) {
	if (*index == BUFFER_MAX_SIZE)
		*index = 0;
}

char ringbuffer_remove(struct ringbuffer* buf) {
	char res = buf->array[buf->tail++];
	fixIndex(&buf->tail);
	return res;
}

void ringbuffer_insert(struct ringbuffer* buf, char data) {
	buf->array[buf->head++] = data;
	fixIndex(&buf->head);
}

void ringbuffer_insert_str(struct ringbuffer* buf, char* str, size_t len) {
	for (size_t i = 0; i < len; i++)
		ringbuffer_insert(buf, str[i]);
}

size_t ringbuffer_get_size(struct ringbuffer* buf) {
	if (buf->head >= buf->tail)
		return (buf->head - buf->tail);
	else
		return BUFFER_MAX_SIZE - buf->tail + buf->head;
}

void ringbuffer_purge(struct ringbuffer* buf) {
	buf->tail = buf->head;
}

char ringbuffer_is_empty(struct ringbuffer* buf) {
	return buf->head == buf->tail;
}

char ringbuffer_is_full(struct ringbuffer* buf) {
	return ringbuffer_get_size(buf) == BUFFER_MAX_SIZE - 1;
}