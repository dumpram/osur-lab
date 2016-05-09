/*! Generate segmentation fault */

#include <stdio.h>
#include <api/prog_info.h>
#include <api/malloc.h>
#include <arch/interrupt.h>
#include <arch/processor.h>

#define DUMMY_PRIO 2

#define TEST	1

/*
 * Dio koda test rada LAB-3 a) i b); za ubaciti u:
 * Chapter_03_Interrupts/04_Interrupts/programs/segm_fault/segm_fault.c
 */

#if TEST == 1
static void test1 ( uint irqn )
{
	int cnt;
	printf ( "Interrupt handler routine (start): irqn=%d\n", irqn );

	for ( cnt = 0; cnt < 500000000; cnt++)
		memory_barrier();

	printf ( "Interrupt handler routine (end): irqn=%d\n", irqn );
}
static void test2 ( uint irqn )
{
	int cnt;
	printf ( "Interrupt handler routine (start): irqn=%d\n", irqn );

	for ( cnt = 0; cnt < 500000000; cnt++)
		memory_barrier();

	printf ( "Interrupt handler routine (end): irqn=%d\n", irqn );
}
static void test3 ( uint irqn )
{
	int cnt;
	printf ( "Interrupt handler routine (start): irqn=%d\n", irqn );

	for ( cnt = 0; cnt < 500000000; cnt++)
		memory_barrier();
	raise_interrupt ( SOFTWARE_INTERRUPT );
	printf ( "Interrupt handler routine (cont): irqn=%d\n", irqn );
	for ( cnt = 0; cnt < 500000000; cnt++)
		memory_barrier();
	raise_interrupt ( SOFTWARE_INTERRUPT-1 );
	printf ( "Interrupt handler routine (cont): irqn=%d\n", irqn );
	for ( cnt = 0; cnt < 500000000; cnt++)
		memory_barrier();

	printf ( "Interrupt handler routine (end): irqn=%d\n", irqn );
}
#endif

int segm_fault ()
{
#if TEST == 1
	printf ( "\nInterrupt test >>>\n" );

	arch_register_interrupt_handler ( SOFTWARE_INTERRUPT,   test1, 10 );
	arch_register_interrupt_handler ( SOFTWARE_INTERRUPT-1, test2, 30 );
	arch_register_interrupt_handler ( SOFTWARE_INTERRUPT-2, test3, 20 );

	raise_interrupt ( SOFTWARE_INTERRUPT-2 );

	printf ( "Interrupt test <<<\n\n" );

#elif TEST == 2
	void *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;

	ptr1 = malloc ( 100 );
	printf ( "malloc returned %x (100)\n", ptr1 );

	ptr2 = malloc ( 500 );
	printf ( "malloc returned %x (500)\n", ptr2 );

	ptr3 = malloc ( 100 );
	printf ( "malloc returned %x (100)\n", ptr3 );

	ptr4 = malloc ( 1000 );
	printf ( "malloc returned %x (1000)\n", ptr4 );

	ptr5 = malloc ( 100 );
	printf ( "malloc returned %x (100)\n", ptr5 );

	if ( ptr2 )
		free (ptr2);
	if ( ptr4 )
		free (ptr4);

	ptr2 = malloc ( 500 );
	printf ( "malloc returned %x (500)\n", ptr2 );

	if ( ptr1 )
		free (ptr1);
	if ( ptr2 )
		free (ptr2);
	if ( ptr3 )
		free (ptr3);
	if ( ptr5 )
		free (ptr5);
#else
	unsigned int *p;
	unsigned int i, j=0;

	printf ( "Example program: [%s:%s]\n%s\n\n", __FILE__, __FUNCTION__,
		 segm_fault_PROG_HELP );

	printf ( "Before segmentation fault\n" );

	for ( i = 16; i < 32; i++ )
	{
		p = (unsigned int *) (1 << i);
		printf ( "[%x]=%d\n", p, *p );
		j+= *p;
	}

	printf ( "After expected segmentation fault, j=%d\n", j );
#endif
	return 0;
}
