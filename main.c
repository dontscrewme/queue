#include "queue.h"
#include "assert.h"

#define MAX 5

struct person {
  unsigned age;
};

void printPerson(struct person* data, FILE* outfile) {
  fprintf(outfile, "age: %d, ", data->age); 
}

void testQueue() {
  queue_t* q = newQueue(MAX, sizeof(struct person)); 

  struct person p;
  
  //dequeue empty queue
  assert(dequeue(q, &p) == -1);
  
  //enqueue MAX times
  for (unsigned i=0; i<MAX; i++) {
    p.age = i;
    assert(enqueue(q, &p) == 0);
  }
  //equeue again
  assert(enqueue(q, &p) == -1);

  //resize queue
  assert(resizeQueue(q, MAX-1) == NULL);
  q = resizeQueue(q, MAX+1);
  assert(q != NULL);
  assert(queueSize(q) == MAX+1);

  //dequeue MAX times
  for (int i = 0; i < MAX; i++) {
    assert(dequeue(q, &p) == 0);
    assert(p.age == i);
  }

  //dequeue again
  assert(dequeue(q, &p) == -1);

  //enqueue MAX+1 times
  for (unsigned i=0; i<MAX+1; i++) {
    p.age = i;
    assert(enqueue(q, &p) == 0);
  }
  //equeue again
  assert(enqueue(q, &p) == -1);

  //dequeue MAX+1 times
  for (int i = 0; i < MAX+1; i++) {
    assert(dequeue(q, &p) == 0);
    assert(p.age == i);
  }

  //dequeue again
  assert(dequeue(q, &p) == -1);
  
  deleteQueue(q);
}

int main(void) {

  testQueue();

  return 0;
}
