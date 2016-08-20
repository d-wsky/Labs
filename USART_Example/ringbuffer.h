#include <stdlib.h>

#define BUFFER_MAX_SIZE   ((size_t) 16)

struct ringbuffer {
    size_t head;
    size_t tail;
    char   array[BUFFER_MAX_SIZE];
};

void ringbuffer_init(struct ringbuffer*);
char ringbuffer_remove(struct ringbuffer*);
void ringbuffer_insert(struct ringbuffer*, char data);
void ringbuffer_insert_str(struct ringbuffer*, char* str, size_t len);
size_t ringbuffer_get_size(struct ringbuffer*);
void ringbuffer_purge(struct ringbuffer*);
char ringbuffer_is_empty(struct ringbuffer*);
char ringbuffer_is_full(struct ringbuffer*);
