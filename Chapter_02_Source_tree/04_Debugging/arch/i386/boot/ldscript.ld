/*! linker script for memory layout */

/* Its parsed as C before used in linking! */

ENTRY(arch_startup)

x_addr = 0x150000;
f_addr = 0x160000;
prog_addr = 0x200000;
exec_addr = 0x300000;


SECTIONS {
	.code LOAD_ADDR :
	{
		kernel_code_addr = .;

		/* instructions */
		*?/boot/startup.asm.o ( .text* )

		*( EXCLUDE_FILE (*?/programs/?*/?*.o) .text* )
	}
	.data :
	{
		kernel_data_addr = .;

		/* read only data (constants), initialized global variables */
		*( EXCLUDE_FILE (*?/programs/?*/?*.o) .rodata* .data* )
	}
	.bss :
	{
		*( EXCLUDE_FILE (*?/programs/?*/?*.o) .bss* COMMON* )

		. = ALIGN (4096);
	}
    .xvars 0x150000 :
    {
        *( .varx* )
    }
    .xfuns 0x160000 :
    {
        *( .funx* )
    }
    .programs exec_addr : AT(prog_addr)
    {
        *?/programs/?*/?*.o ( .text* .rodata* .data* .bss* )
    }

	/*
	 * what with other sections generated with gcc (various versions)?
	 * if optimizing for size discard them with: /DISCARD/ : { *(*) }
	 * if debugging with qemu/gdb they must be included
	 * - they can be included implicitly, without declaring them here
	 * - but they will be put behind "kernel_end_addr" so its best to use
	 *   multiboot loader (QEMU is that)
	 */

#ifndef DEBUG
	/DISCARD/ : { *(*) }
#endif
	kernel_end_addr = .;
}
