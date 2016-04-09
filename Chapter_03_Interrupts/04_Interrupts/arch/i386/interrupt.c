/*! Interrupt handling - 'arch' layer (only basic operations) */

#define _ARCH_INTERRUPTS_C_
#include "interrupt.h"

#include <arch/processor.h>
#include <kernel/errno.h>
#include <lib/list.h>
#include <kernel/memory.h>

/*! Interrupt controller device */
extern arch_ic_t IC_DEV;
static arch_ic_t *icdev = &IC_DEV;

/*! interrupt handlers */
static list_t ihandlers[INTERRUPTS];

struct ihndlr
{
	int (*ihandler) ( unsigned int );

	list_h list;

    int priority;

    int req_cnt;

    int is_processed;
};

/*! Initialize interrupt subsystem (in 'arch' layer) */
void arch_init_interrupts ()
{
	int i;

	icdev->init ();

	for ( i = 0; i < INTERRUPTS; i++ )
		list_init ( &ihandlers[i] );
}

/*!
 * enable and disable interrupts generated outside processor, controller by
 * interrupt controller (PIC or APIC or ...)
 */
void arch_irq_enable ( unsigned int irq )
{
	icdev->enable_irq ( irq );
}
void arch_irq_disable ( unsigned int irq )
{
	icdev->disable_irq ( irq );
}

/*! Register handler function for particular interrupt number */
void arch_register_interrupt_handler ( unsigned int inum, void *handler,
     int priority )
{
	struct ihndlr *ih;

	if ( inum < INTERRUPTS )
	{
		ih = kmalloc ( sizeof (struct ihndlr) );
		ASSERT ( ih );

		ih->ihandler = handler;

        ih->priority = priority;

        ih->is_processed = 0;

		list_append ( &ihandlers[inum], ih, &ih->list );
	}
	else {
		LOG ( ERROR, "Interrupt %d can't be used!\n", inum );
		halt ();
	}
}

/*! Unregister handler function for particular interrupt number */
void arch_unregister_interrupt_handler ( unsigned int irq_num, void *handler )
{
	struct ihndlr *ih, *next;

	ASSERT ( irq_num >= 0 && irq_num < INTERRUPTS );

	ih = list_get ( &ihandlers[irq_num], FIRST );

	while ( ih )
	{
		next = list_get_next ( &ih->list );

		if ( ih->ihandler == handler )
			list_remove ( &ihandlers[irq_num], FIRST, &ih->list );

		ih = next;
	}
}

/*!
 * "Forward" interrupt handling to registered handler
 * (called from interrupts.S)
 */
void arch_interrupt_handler ( int irq_num )
{
	struct ihndlr *ih;

    struct ihndlr *max_prio_ih;

    int max_prio = 0;

	if(irq_num < INTERRUPTS && (ih = list_get (&ihandlers[irq_num], FIRST)))
	{
		/* enable interrupts on PIC immediately since program may not
		 * return here immediately */
		if ( icdev->at_exit )
			icdev->at_exit ( irq_num );

        while ( ih ) {
            ih->req_cnt++;
            ih = list_get_next ( &ih->list );
        }

        max_prio_ih = ih = list_get (&ihandlers[irq_num], FIRST);

		while ( max_prio_ih ) {
            max_prio_ih = NULL;
    		while ( ih )
    		{
                if ( ih->priority > max_prio && ih->req_cnt != 0 ) {
                    max_prio_ih = ih;
                    max_prio = ih->priority;
                }
    			//ih->ihandler ( irq_num );
    			ih = list_get_next ( &ih->list );
    		}
            if ( !max_prio_ih ) {
                break;
            }
            if ( max_prio_ih->is_processed ) {
                return;
            } else {
                max_prio_ih->is_processed = 1;
                max_prio_ih->ihandler( irq_num );
                max_prio_ih->req_cnt--;
                max_prio_ih->is_processed = 0;
            }
            max_prio = 0;
            max_prio_ih = ih = list_get (&ihandlers[irq_num], FIRST);
        }

	}

	else if ( irq_num < INTERRUPTS )
	{
		LOG ( ERROR, "Unregistered interrupt: %d - %s!\n",
		      irq_num, icdev->int_descr ( irq_num ) );
		halt ();
	}
	else {
		LOG ( ERROR, "Unregistered interrupt: %d !\n", irq_num );
		halt ();
	}
}
