#include <stdio.h>
#include <lib/queue.h>



int q_test () {
    queue_t *q = queue_init(20);
    if ( queue_isempty (q)) {
        printf("queue_isempty: SUCCESS!\n");
    }
    queue_enqueue(q, 'a');
    if ( queue_dequeue( q ) == 'a') {
        printf("queue_en, deq: SUCCESS!");
    }
    return 0;
}
