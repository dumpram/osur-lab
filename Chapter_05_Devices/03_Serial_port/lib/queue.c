#include <lib/queue.h>

queue_t *queue_init ( int size ) {
        queue_t *q = malloc (sizeof(queue_t));
        if ( q == NULL ) {
            return NULL;
        }
        q->size = size;
        q->front = -1;
        q->rear = -1;
        q->data = malloc(sizeof(char) * size);
        return q;
}

void queue_enqueue ( queue_t *q, char ch ) {
    if ( queue_isfull(q) ) {
        return;
    }
    q->rear = (q->rear + 1) % (q->size);
    q->data[q->rear] = ch;
    if (q->front == -1) {
        q->front = q->rear;
    }
}

char queue_dequeue ( queue_t *q ) {
    char ch;
    if ( queue_isempty(q) ) {
        return -1;
    }
    ch = q->data[q->front];
    if ( q->front == q->rear ) {
        q->front = q->rear = -1;
    } else {
        q->front = (q->front + 1) % (q->size);
    }
    return ch;
}

queue_t *queue_resize ( queue_t *q_old, size_t new_size ) {
    queue_t *q_new = queue_init (new_size);
    while (!queue_isempty (q_old)) {
        queue_enqueue (q_new, queue_dequeue(q_old));
    }
    free (q_old);
    return q_new;
}

int queue_isfull ( queue_t *q ) {
    return (q->rear + 1) % ( q->size ) == q->rear;
}

int queue_isempty ( queue_t *q ) {
    return (q->front == -1);
}
