#ifndef _PIPE_H_
#define _PIPE_H_

#include <kernel/pipe.h>

int create_pipe ( char *name ) {
    return __sys_create_pipe ( char *name );
}


#endif /* end of include guard: _PIPE_H_ */
