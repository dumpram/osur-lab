#include <arch/device.h>
#include <kernel/memory.h>
#include <kernel/pipe.h>
#include <kernel/device.h>
#include "device.h"
#include <lib/queue.h>
#include <lib/string.h>
#include <kernel/errno.h>
#include <kernel/kprint.h>
#include "device.h"

#define INIT_SIZE 0

typedef struct _pipe_params_t pipe_params_t;

struct _pipe_params_t {
    queue_t *pipe;
};

static int pipe_init ( uint flags, void *params, device_t *dev ) {
    pipe_params_t *pipe_params;
    pipe_params = (pipe_params_t *)kmalloc(sizeof(pipe_params_t));
    pipe_params->pipe = queue_init (INIT_SIZE);
    dev->params = pipe_params;
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
            //kprintf("QUEUE IS FULL (rear = %d, front = %d)\n", q->rear, q->front);
            q = queue_resize (q, q->size + size + 1);
            ((pipe_params_t *)dev->params)->pipe = q;
            //check success of malloc
        }// else {
        //     queue_enqueue (q, *(c_data++));
        // }
        queue_enqueue (q, *(c_data++));
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
    .status = pipe_status,
    .irq_handler = NULL

    /* SENTINEL */
};

int k_create_pipe ( char *name ) {
    // add pipe to list of devices
    // initialize everything
    kdevice_t *kdev;
    device_t *pipe_new = malloc(sizeof(device_t));
    *pipe_new = pipe;

    memcpy (pipe_new->dev_name, name, strlen(name));
    kdev = k_device_add ( pipe_new );

    if (kdev == NULL) {
        return -1;
    }
    k_device_init ( kdev, 0, NULL, NULL );
    return 0;
}



int sys__create_pipe ( char *name ) {
    int status, errno;

    SYS_ENTRY();

    status = k_create_pipe ( name );
    errno = (status == EXIT_FAILURE)? EEXIST : EXIT_SUCCESS;

    SYS_EXIT( errno , status );

}

int sys__delete_pipe ( char *name ) {

    SYS_ENTRY();

    k_device_delete ( name );

    SYS_EXIT( EXIT_SUCCESS, EXIT_SUCCESS );

}
