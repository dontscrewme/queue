#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include "queue.h"

typedef struct thread_safe_queue ts_queue_t;

ts_queue_t *newQueueTS(unsigned size, unsigned elementSize);
void deleteQueueTS(ts_queue_t *tsq);
int enqueueTS(ts_queue_t *tsq, const void *item);
int dequeueTS(ts_queue_t *tsq, void *item);
int getSizeTS(ts_queue_t *tsq, unsigned int* output);
int getNumOfElementsTS(ts_queue_t *tsq, unsigned int* output);
int getNumOfEmptySlotsTS(ts_queue_t *tsq, unsigned int* output);
int resizeQueueTS(ts_queue_t *tsq, unsigned newSize);

void queueTS_set_error_callback(int (*errback)(const char *, ...));
int printQueueTS(ts_queue_t *tsq, FILE *outfile, void (*printFunc)(const void *data, FILE *outfile));
#endif
