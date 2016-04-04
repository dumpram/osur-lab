/*! Startup function - initialize kernel subsystem */
#define _K_STARTUP_C_

#include <arch/processor.h>
#include <api/stdio.h>
#include <api/prog_info.h>
#include <types/io.h>
#include <kernel/errno.h>


/*! test includes */
#include <kernel/test.h>

/*! kernel stack */
uint8 system_stack [ STACK_SIZE ];

char system_info[] = 	OS_NAME ": " NAME_MAJOR ":" NAME_MINOR ", "
			"Version: " VERSION " (" ARCH ")";
/*!
* Funkcija priprema programe za izvodjenje.
*/
static void prog_prepare()
{
    extern char prog_rom_addr, prog_exec_addr, prog_end_addr;
    char *src, *dst, *end;

    /* priprema pokazivača */
    src = &prog_rom_addr;
    dst = &prog_exec_addr;
    end = &prog_end_addr;
    while(src < end) {
        *dst++ = *src++;
    }
}
/*!
 * First kernel function (after boot loader loads it to memory)
 */
void k_startup ()
{
	extern console_t K_INITIAL_STDOUT, K_STDOUT;
    extern console_t *k_stdout; /* console for kernel messages */

	/* set initial stdout */
	k_stdout = &K_INITIAL_STDOUT;
	k_stdout->init (0);

	/*! start with regular initialization */

	/* switch to default 'stdout' for kernel */
	k_stdout = &K_STDOUT;
	k_stdout->init (0);

	kprintf ( "%s\n", system_info );

    /* ispis vrijednosti varijable i adresa testne varijable i funkcije */
    ispisi_x();

    /* initialize standard output devices */
    stdio_init ();

    /* priprema programa */
    prog_prepare();

	/* start desired program(s) */
    hello_world ();
	debug ();

	kprintf ( "\nSystem halted!\n" );
	halt ();
}
