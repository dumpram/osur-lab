#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct _queue_t queue_t;

struct queue_t {
    int size;
    char *wptr;
    char *rptr;
    char *data;
}

queue_t *createQueue(int queue_size);

char readQueue();

char writeQueue();

#endif /* end of include guard: _QUEUE_H_  */
