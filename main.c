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
  queue_t* old_q = q;
  assert(resizeQueue(q, MAX-1) == old_q);
  q = resizeQueue(q, MAX+1);
  assert(q != old_q && q != NULL);
  assert(sizeOfQueue(q) == MAX+1);

  //dequeue MAX times
  for (int i = 0; i < MAX; i++) {
    assert(dequeue(q, &p) == 0);
    assert(p.age == i);
  }

  //dequeue again
  assert(dequeue(q, &p) == -1);

  // enqueue MAX+1 times
  for (unsigned i=0; i<MAX+1; i++) {
    p.age = i;
    assert(enqueue(q, &p) == 0);
  }
  //equeue again
  assert(enqueue(q, &p) == -1);

  // dequeue MAX+1 times
  for (int i = 0; i < MAX+1; i++) {
    assert(dequeue(q, &p) == 0);
    assert(p.age == i);
  }

  // dequeue again
  assert(dequeue(q, &p) == -1);

  deleteQueue(q);


  q = newQueue(MAX, sizeof(struct person)); 


  //tail before head
  for (unsigned i=0; i<MAX-1; i++) {
    p.age = i;
    assert(enqueue(q, &p) == 0);
  }
  assert(dequeue(q, &p) == 0);
  assert(dequeue(q, &p) == 0);
  for (unsigned i=0; i<2; i++) {
    p.age = i;
    assert(enqueue(q, &p) == 0);
  }

  old_q = q;
  
  assert(resizeQueue(q, 3) == old_q);

  q = resizeQueue(q, 4);
  assert(q != old_q && q != NULL);
  
  deleteQueue(q);
}

int main(void) {

  testQueue();

  return 0;
}
