#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdio.h>

typedef struct queue_t queue_t;

queue_t *newQueue(unsigned size, unsigned elementSize);
void deleteQueue(queue_t *me);
int enqueue(queue_t *me, const void *item);
int dequeue(queue_t *me, void *item);
int getSize(queue_t *me, unsigned int *output);
int getNumOfElements(queue_t *me, unsigned int *output);
int getNumOfEmptySlots(queue_t *me, unsigned int *output);
int resizeQueue(queue_t *me, unsigned newSize, queue_t **newQueue);

extern int (*queue_error_callback)(const char *, ...);
void queue_set_error_callback(int (*errback)(const char *, ...));
int printQueue(queue_t *me, FILE *outfile,
               void (*printFunc)(const void *data, FILE *outfile));

#endif
