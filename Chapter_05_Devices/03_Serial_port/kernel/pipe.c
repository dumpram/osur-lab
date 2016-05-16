#include <arch/device.h>
#include <kernel/memory.h>
#include <kernel/pipe.h>
#include <lib/queue.h>

#define INIT_SIZE 16

typedef struct _pipe_params_t pipe_params_t;

struct _pipe_params_t {
    queue_t *pipe;
};

static int pipe_init ( uint flags, void *params, device_t *dev ) {
    pipe_params_t *pipe_params;
    pipe_params = (pipe_params_t *)kmalloc(sizeof(pipe_params_t));
    pipe_params->pipe = queue_init (INIT_SIZE);
    dev->params = params;
    return 0;
}

static int pipe_destroy ( uint flags, void *params, device_t *dev ) {
    pipe_params_t *pipe_params = (pipe_params_t *) dev->params;
    free (pipe_params->pipe);
    return 0;
}

static int pipe_send ( void *data, size_t size, uint flags, device_t *dev) {
    char *c_data = (char *) data;
    pipe_params_t *pipe_params = (pipe_params_t *)dev->params;
    queue_t *q = (queue_t *)pipe_params->pipe;
    while ( size-- ) {
        if ( queue_isfull (q) ) {
            q = queue_resize (q, 2 * q->size);
            //check success of malloc
        } else {
            queue_enqueue (q, *(c_data++));
        }
    }
    return 0;
}

static int pipe_recv ( void *data, size_t size, uint flags, device_t *dev ) {


    //check if size is larger than size of pipe, recieve data pipe_size

    char *c_data = (char *) data;
    pipe_params_t *pipe_params = (pipe_params_t *)dev->params;
    queue_t *q = (queue_t *)pipe_params->pipe;
    while ((size--) && !queue_isempty ( q )) {
        *(c_data++) = queue_dequeue ( q );
    }
    return 0;
}

static int pipe_status ( uint flags, device_t *dev ) {
    return 0;
}

device_t pipe = {

    .init = pipe_init,
    .destroy = pipe_destroy,
    .send = pipe_send,
    .recv = pipe_recv,
    .status = pipe_status

    /* SENTINEL */
};

int __sys_create_pipe ( char *name ) {
    
    return 0;
}
