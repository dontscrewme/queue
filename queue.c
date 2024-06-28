#include "queue.h"

struct queue_t {
  unsigned head;
  unsigned tail;
  unsigned size;
  unsigned isEmpty;
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
    queue_error_callback("%s: malloc failed\n", __func__);
    return NULL;
  }

  me->head = 0;
  me->tail = 0;
  me->isEmpty = 1;
  me->elementSize = elementSize;
  me->size = size;
  memset(me->data, 0, elementSize * size);

  return me;
}

void deleteQueue(queue_t *me) {
  if (!me) {
    return;
  }

  free(me);
}

int enqueue(queue_t *me, const void *item) {
  if (!me || !item) {
    queue_error_callback("%s: invalid input\n", __func__);
    return -1;
  }

  if (numOfElements(me) == me->size) {
    queue_error_callback("%s: queue is full\n", __func__);
    return -1;
  }

  if (me->isEmpty) {
    me->isEmpty = 0;
  }
  else {
    if (++me->tail == me->size) {
      me->tail = 0;
    }
  }

  memcpy(&me->data[me->tail * me->elementSize], item, me->elementSize);

  return 0;
}

int dequeue(queue_t *me, void *item) {
  if (!me || !item) {
    queue_error_callback("%s: invalid input\n", __func__);
    return -1;
  }

  if (me->isEmpty) {
    queue_error_callback("%s: queue is empty\n", __func__);
    return -1;
  }

  memcpy(item, &me->data[me->head * me->elementSize], me->elementSize);

  if (me->head == me->tail) {
    me->isEmpty = 1;
  }
  else {
    if (++me->head == me->size) {
      me->head = 0;
    }
  }

  return 0;
}

int queueSize(queue_t *me) {
  if (!me) {
    queue_error_callback("%s: invalid input\n", __func__);
    return -1;
  }

  return me->size;
}

int numOfElements(queue_t *me) {
  if (!me) {
    queue_error_callback("%s: invalid input\n", __func__);
    return -1;
  }

  if (me->isEmpty) {
    return 0;
  }

  if (me->tail >= me->head) {
    return me->tail - me->head + 1;
  }

  return me->tail - me->head + me->size + 1;
}

int numOfEmptySlots(queue_t *me) {
  if (!me) {
    queue_error_callback("%s: invalid input\n", __func__);
    return -1;
  }

  return me->size - numOfElements(me);
}

queue_t* resizeQueue(queue_t* me, unsigned newSize) {
  if (!me || newSize == 0) {
    queue_error_callback("%s: invalid input\n", __func__);
    return NULL;
  }

  if (numOfElements(me) >= newSize) {
    queue_error_callback("%s: new size is too small\n", __func__);
    return NULL;
  }

  queue_t* newMe = newQueue(newSize, me->elementSize);
  if (!newMe) {
    return NULL;
  }

  unsigned firstChunkSize = (me->size - me->head) * me->elementSize;
  memcpy(newMe->data, &me->data[me->head * me->elementSize], firstChunkSize);

  if (me->tail < me->head) {
    unsigned secondChunkSize = (me->tail + 1) * me->elementSize;
    memcpy(&newMe->data[firstChunkSize], me->data, secondChunkSize);
  }

  newMe->tail = numOfElements(me) - 1;
  newMe->isEmpty = me->isEmpty;

  deleteQueue(me);
  
  return newMe;
}

  int printQueue(queue_t *me, FILE* outfile, void(*printFunc)(const void *data, FILE *outfile)) {
  if (!me || !printFunc) {
    queue_error_callback("%s: invalid input\n", __func__);
    return -1;
  }

  if (me->isEmpty) {
    return 0;
  }
  
  unsigned temp_head = me->head;
  int count = numOfElements(me);
  while (count) {
    printFunc(&me->data[temp_head * me->elementSize], outfile);
    temp_head = (temp_head + 1) % me->size;
    count--;
  }

    return 0;
}
