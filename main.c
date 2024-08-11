#include "assert.h"
#include "queue.h"

#define MAX 5

struct person {
  unsigned age;
};

void printPerson(struct person *data, FILE *outfile) {
  fprintf(outfile, "age: %d, ", data->age);
}

void testQueue() {
  queue_t *q = newQueue(MAX, sizeof(struct person));

  struct person p;

  // dequeue empty queue
  assert(dequeue(q, &p) == -1);

  // enqueue MAX times
  for (unsigned i = 0; i < MAX; i++) {
    p.age = i;
    assert(enqueue(q, &p) == 0);
  }

  // equeue again
  assert(enqueue(q, &p) == -1);

  // resize queue
  queue_t *new_queue = NULL;
  unsigned size = 0;
  assert(resizeQueue(q, MAX - 1, &new_queue) == -1);
  assert(q != NULL && new_queue == NULL);
  assert(getSize(q, &size) == 0);
  assert(size == MAX);

  assert(resizeQueue(q, MAX + 1, &new_queue) == 0);
  assert(new_queue != NULL);
  assert(getSize(new_queue, &size) == 0);
  assert(size == MAX + 1);

  unsigned int num_of_elements = 0;
  unsigned int num_of_empty_slots = 0;
  assert(getNumOfElements(new_queue, &num_of_elements) == 0);
  assert(getNumOfEmptySlots(new_queue, &num_of_empty_slots) == 0);
  assert(num_of_elements == MAX);
  assert(num_of_empty_slots == 1);

  // dequeue MAX times
  for (int i = 0; i < MAX; i++) {
    assert(dequeue(new_queue, &p) == 0);
    assert(p.age == i);
  }

  // dequeue again
  assert(dequeue(new_queue, &p) == -1);

  // enqueue MAX+1 times
  for (unsigned i = 0; i < MAX + 1; i++) {
    p.age = i;
    assert(enqueue(new_queue, &p) == 0);
  }
  // equeue again
  assert(enqueue(new_queue, &p) == -1);

  // dequeue MAX+1 times
  for (int i = 0; i < MAX + 1; i++) {
    assert(dequeue(new_queue, &p) == 0);
    assert(p.age == i);
  }

  // dequeue again
  assert(dequeue(new_queue, &p) == -1);

  deleteQueue(new_queue);

  q = newQueue(MAX, sizeof(struct person));

  // tail before head
  for (unsigned i = 0; i < MAX - 1; i++) {
    p.age = i;
    assert(enqueue(q, &p) == 0);
  }
  assert(dequeue(q, &p) == 0);
  assert(dequeue(q, &p) == 0);
  for (unsigned i = 0; i < 2; i++) {
    p.age = i;
    assert(enqueue(q, &p) == 0);
  }

  // resize queue
  new_queue = NULL;
  size = 0;
  assert(resizeQueue(q, MAX - 1, &new_queue) == -1);
  assert(q != NULL && new_queue == NULL);
  assert(getSize(q, &size) == 0);
  assert(size == MAX);

  assert(resizeQueue(q, MAX+1, &new_queue) == 0);
  assert(new_queue != NULL);
  assert(getSize(new_queue, &size) == 0);
  assert(size == MAX+1);

  num_of_elements = 0;
  num_of_empty_slots = 0;
  assert(getNumOfElements(new_queue, &num_of_elements) == 0);
  assert(getNumOfEmptySlots(new_queue, &num_of_empty_slots) == 0);
  assert(num_of_elements == MAX-1);
  assert(num_of_empty_slots == 2);

  deleteQueue(new_queue);
}

int main(void) {

  testQueue();

  return 0;
}
