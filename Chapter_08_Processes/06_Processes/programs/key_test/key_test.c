#include <pthread.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>

char PROG_HELP[] = "Test for pthread_key interface...";

//primjeri podataka
typedef int Status;
typedef int Podaci;

//globalne varijable, dijeljene među dretvama
pthread_key_t kStatus, kPodaci;

void *dretva(void *x);
void funkcija();
void oslobodi(void *x);

int key_test ()
{
	pthread_t t1; //opisnici za nove dretve
    //pthread_t t2;

    //inicijalizacija kljuceva, potrebno za privatne podatke dretve
	pthread_key_create ( &kStatus, oslobodi );
	//pthread_key_create ( &kStatus, oslobodi );
	//inicijalno, NULL vrijednost je povezana uz kljuceve za sve dretve

	pthread_create ( &t1, NULL, dretva, (void *) 1 );
//	pthread_create ( &t2, NULL, dretva, (void *) 2 );

	pthread_join ( t1, NULL );
//	pthread_join ( t2, NULL );

	return 0;
}

//radna dretva - inicijalizacija
void *dretva ( void *x )
{
	Status *stat;
	Podaci *podaci;
	timespec_t sleep;

	stat = malloc ( sizeof(Status) );   //stat - lokalna varijabla
	podaci = malloc ( sizeof(Podaci) ); //podaci - lokalna varijabla

	*stat = (int) x;
	*podaci = *stat * 10;

	//povezi 'stat' s kljucem 'kStatus' u trenutnoj dretvi
	pthread_setspecific ( kStatus, stat );
	//povezi 'podaci' s kljucem 'kPodaci' u trenutnoj dretvi
//	pthread_setspecific ( kPodaci, podaci );

	sleep.tv_sec = 1;
	sleep.tv_nsec = 0;
	nanosleep ( &sleep, NULL );

	funkcija();

	return NULL;
}

//radna dretva – u nekoj funkciji
void funkcija ()
{
	// Status *s;
	// Podaci *b;
    //
	// //dohvati podatke povezane uz kljuceve
	// s = pthread_getspecific ( kStatus );
	// b = pthread_getspecific ( kPodaci );
    //
	// //koristi ‘s’ i ‘b’
	// printf ( "stat=%d, podaci=%d\n", *s, *b );

    //printf ("Poziv funkcije!\r\n");
}

void oslobodi ( void *x )
{
	//printf("Oslobadjanje %p\n", x);
	// if (x)
	// 	free(x);

    //printf ("Poziv destruktora...!\r\n");
}
