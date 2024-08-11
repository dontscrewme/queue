#include "queue.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct queue_t {
  unsigned head;
  unsigned tail;
  unsigned size;
  unsigned numOfElements;
  unsigned elementSize;
  char data[];
};

static int default_error_callback(const char *format, ...) {
  int ret;
  va_list argptr;
  va_start(argptr, format);
  ret = vfprintf(stderr, format, argptr);
  va_end(argptr);
  return ret;
}

int (*queue_error_callback)(const char *, ...) = default_error_callback;

void queue_set_error_callback(int (*errback)(const char *, ...)) {
  if (errback) {
    queue_error_callback = errback;
  }
}

queue_t *newQueue(unsigned size, unsigned elementSize) {
  if (size == 0 || elementSize == 0) {
    queue_error_callback("%s: invalid input\n", __func__);
    return NULL;
  }

  queue_t *me = malloc(sizeof(queue_t) + (size * elementSize));
  if (!me) {
    queue_error_callback("%s: malloc() failed\n", __func__);
    return NULL;
  }

  me->head = 0;
  me->tail = 0;
  me->size = size;
  me->numOfElements = 0;
  me->elementSize = elementSize;
  memset(me->data, 0, elementSize * size);

  return me;
}

void deleteQueue(queue_t *me) {
  if (!me) {
    queue_error_callback("%s: invalid input\n", __func__);
    return;
  }

  free(me);
}

int enqueue(queue_t *me, const void *item) {
  if (!me || !item) {
    queue_error_callback("%s: invalid input\n", __func__);
    return -1;
  }

  if (me->numOfElements == me->size) {
    queue_error_callback("%s: queue is full\n", __func__);
    return -1;
  }

  memcpy(&me->data[me->tail * me->elementSize], item, me->elementSize);

  if (++me->tail == me->size) {
    me->tail = 0;
  }

  me->numOfElements++;

  return 0;
}

int dequeue(queue_t *me, void *item) {
  if (!me || !item) {
    queue_error_callback("%s: invalid input\n", __func__);
    return -1;
  }

  if (me->numOfElements == 0) {
    queue_error_callback("%s: queue is empty\n", __func__);
    return -1;
  }

  memcpy(item, &me->data[me->head * me->elementSize], me->elementSize);

  if (++me->head == me->size) {
    me->head = 0;
  }

  me->numOfElements--;

  return 0;
}

int getSize(queue_t *me, unsigned int *output) {
  if (!me || !output) {
    queue_error_callback("%s: invalid input\n", __func__);
    return -1;
  }

  *output = me->size;

  return 0;
}

int getNumOfElements(queue_t *me, unsigned int *output) {
  if (!me || !output) {
    queue_error_callback("%s: invalid input\n", __func__);
    return -1;
  }

  *output = me->numOfElements;

  return 0;
}

int getNumOfEmptySlots(queue_t *me, unsigned int *output) {
  if (!me || !output) {
    queue_error_callback("%s: invalid input\n", __func__);
    return -1;
  }

  *output = me->size - me->numOfElements;

  return 0;
}

int resizeQueue(queue_t *me, unsigned newSize, queue_t** output) {
  if (!me) {
    queue_error_callback("%s: invalid input\n", __func__);
    return -1;
  }

  if (me->size >= newSize) {
    queue_error_callback("%s: new size is too small\n", __func__);
    return -1;
  }

  queue_t *newMe = newQueue(newSize, me->elementSize);
  if (!newMe) {
    queue_error_callback("%s: newQueue() failed\n ", __func__);
    return -1;
  }

  unsigned firstChunkSize = (me->size - me->head) * me->elementSize;
  if (memcpy(newMe->data, &me->data[me->head * me->elementSize], firstChunkSize) == NULL) {
    queue_error_callback("%s: memcpy failed\n", __func__);
    deleteQueue(newMe);
    return -1;
  }

  if (me->tail < me->head && me->tail != 0) {
    unsigned secondChunkSize = me->tail * me->elementSize;
    if (memcpy(&newMe->data[firstChunkSize], me->data, secondChunkSize) == NULL) {
      queue_error_callback("%s: memcpy failed\n", __func__);
      deleteQueue(newMe);
      return -1;
    }
  }

  newMe->numOfElements = me->numOfElements;
  newMe->tail = me->numOfElements;

  deleteQueue(me);

  *output = newMe;

  return 0;
}

int printQueue(queue_t *me, FILE *outfile,
               void (*printFunc)(const void *data, FILE *outfile)) {
  if (!me || !printFunc) {
    queue_error_callback("%s: invalid input\n", __func__);
    return -1;
  }

  if (me->numOfElements == 0) {
    return 0;
  }

  unsigned temp_head = me->head;
  int count = me->numOfElements;
  while (count) {
    fprintf(outfile, "(%d)", temp_head);
    printFunc(&me->data[temp_head * me->elementSize], outfile);
    if (++temp_head == me->size) {
      temp_head = 0;
    }
    count--;
  }
  fprintf(outfile, "\n");

  return 0;
}
