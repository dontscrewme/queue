#include "threadSafeQueue.h"
#include <pthread.h>
#include <stdlib.h>

#include <stdatomic.h>

struct thread_safe_queue {
  queue_t *queue;
  pthread_mutex_t mutex;
  pthread_cond_t queue_not_full;
  pthread_cond_t queue_not_empty;
};

void queueTS_set_error_callback(int (*errback)(const char *, ...)) {
  queue_set_error_callback(errback);
}

ts_queue_t *newQueueTS(unsigned size, unsigned elementSize) {

  ts_queue_t *tsq = malloc(sizeof(ts_queue_t));
  if (!tsq) {
    queue_error_callback("%s: malloc() failed\n", __func__);
    goto tsq_failed;
  }

  tsq->queue = newQueue(size, elementSize);
  if (!tsq->queue) {
    queue_error_callback("%s: newQueue() failed\n", __func__);
    goto queue_failed;
  }

  if (pthread_mutex_init(&tsq->mutex, NULL) != 0) {
    queue_error_callback("%s: mutex_init failed\n", __func__);
    goto mutex_failed;
  }

  if (pthread_cond_init(&tsq->queue_not_full, NULL) != 0) {
    queue_error_callback("%s: queue_not_full_cond_init failed\n", __func__);
    goto queue_not_full_failed;
  }

  if (pthread_cond_init(&tsq->queue_not_empty, NULL) != 0) {
    queue_error_callback("%s: queue_not_empty_cond_init failed\n", __func__);
    goto queue_not_empty_failed;
  }

  return tsq;

queue_not_empty_failed:
  pthread_cond_destroy(&tsq->queue_not_full);

queue_not_full_failed:
  pthread_mutex_destroy(&tsq->mutex);

mutex_failed:
  deleteQueue(tsq->queue);

queue_failed:
  free(tsq);

tsq_failed:
  return NULL;
}

void deleteQueueTS(ts_queue_t *tsq) {
  if (!tsq) {
    return;
  }

  pthread_cond_destroy(&tsq->queue_not_empty);
  pthread_cond_destroy(&tsq->queue_not_full);
