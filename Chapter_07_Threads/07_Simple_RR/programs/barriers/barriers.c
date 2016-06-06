/*! Barriers example (threads) */

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#define NUM_OF_ITER 2

char PROG_HELP[] = "Thread example (barriers): custom barrier example.";

#define BARR_CNT 10

pthread_barrier_t test_barr;


static void *test_thread ( void *param )
{
    int thread_id = (int) param;
    int i;
    int iter = NUM_OF_ITER;

    while (iter--) {
        i = thread_id * 1000000;
        while (i--);
        printf ("Thread %d reached barrier\n", thread_id);
        pthread_barrier_wait ( &test_barr );
        printf ("Thread %d crossed barrier\n", thread_id);
    }
    return NULL;
}

int barriers ( char *args[] ) {

    int i;
    pthread_t thread[BARR_CNT];
    pthread_barrier_init ( &test_barr, 0, BARR_CNT );

    printf ( "Example: [%s:%s]\n%s\n\n", __FILE__, __FUNCTION__,
         PROG_HELP );

    for ( i = 0; i < BARR_CNT; i++ ) {
        pthread_create ( &thread[i], NULL,
				 test_thread, (void *) (i+1) );
    }

    for ( i = 0; i < BARR_CNT; i++ ) {
        pthread_join ( thread[i], NULL );
    }

    printf ( "Destroying barrier..." );
    pthread_barrier_destroy ( &test_barr );

    return 0;
}
