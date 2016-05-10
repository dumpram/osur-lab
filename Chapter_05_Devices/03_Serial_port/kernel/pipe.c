#include <arch/device.h>
#include <kernel/memory.h>

#define INIT_SIZE 16

typedef struct _pipe_params_t pipe_params_t;

static int current_fd;

struct _pipe_params_t {
    int fd;
    int size;
    char *input;
    char *output;
    char *buffer;
};

static int pipe_init ( uint flags, void *params, device_t *dev ) {
    pipe_params_t *pipe_params;
    pipe_params = (pipe_params_t *)kmalloc(sizeof(pipe_params_t) - \
                sizeof(char *) + INIT_SIZE);
    pipe_params->fd = current_fd++;
    pipe_params->input = buffer;
    pipe_params->output = buffer;
    return 0;
}

static int pipe_destroy ( uint flags, void *params, device_t *dev ) {
    return 0;
}

static int pipe_send ( void *data, size_t size, uint flags, device_t *dev) {
    char *c_data = (char *) data;
    pipe_params_t *pipe_params = (pipe_params_t *)dev->params;
    int i;
    for (i = 0; i < size; i++) {
        *(pipe_params->input) = c_data[i];
        pipe_params->input++;
    }
    return 0;
}

static int pipe_recv ( void *data, size_t size, uint flags, device_t *dev ) {

    data = (char *) dev->para

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
