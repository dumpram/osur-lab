/*! Barriers example (threads) */

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

char PROG_HELP[] = "Thread example (spinlocks): fibbo spinlock example.";

#define SPIN_CNT 10

int thread_counter = 0;

pthread_spinlock_t test_lock;

static void *test_thread ( void *param )
{
    int thread_id = (int) param;
    int i = thread_id * 1000000;

    pthread_spin_lock ( &test_lock );

    thread_counter += 1;

    printf ( "Thread %d started\n", thread_counter );

    while(i--);

    printf ( "Thread %d finished\n");

    pthread_spin_unlock ( &test_lock );

    return NULL;
}


int spinlocks ( char *args[] ) {

    int i;
    pthread_t thread[SPIN_CNT];
    pthread_spin_init ( &test_lock );

    printf ( "Example: [%s:%s]\n%s\n\n", __FILE__, __FUNCTION__,
         PROG_HELP );

    for ( i = 0; i < SPIN_CNT; i++ ) {
        pthread_create ( &thread[i], NULL,
				 test_thread, (void *) (i+1) );
    }

    for ( i = 0; i < SPIN_CNT; i++ ) {
        pthread_join ( thread[i], NULL );
    }

    return 0;
}
