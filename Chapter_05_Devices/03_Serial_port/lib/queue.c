#include <lib/queue.h>

queue_t *createQueue(int queue_size) {
    queue_t *new;
    new = (queue_t *)malloc (sizeof(queue_t));
    new->data = (char *)malloc (sizeof(char) * size);
    new->size = size;
    new->rptr = NULL;
    new->wptr = NULL;
}

int readQueue(queue_t *q, char *data) {
    char forExport;
    if (q->rptr == NULL) {
        return -1;
    }
    forExport = *(q->rptr);
    if (q->rptr - q->data > q->size) {

    }
    q->rptr++;
    return forExport;
}

void writeQueue(char data, queue_t *q) {
    *(q->wptr) = data;
    q->wptr++;
}
