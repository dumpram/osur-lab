/*! Hello world program */

#include <stdio.h>
#include <lib/string.h>
#include <api/prog_info.h>

int hello_world ()
{
	printf ( "Example program: [%s:%s]\n%s\n\n", __FILE__, __FUNCTION__,
		 hello_world_PROG_HELP );

	printf ( "Hello World!\n" );

	/* pipe test */
	create_pipe ( "A" );
	create_pipe ( "B" );

	int fd1 = open ( "A", 0, 0 );
	int fd2 = open ( "B", 0, 0 );

	printf ( "fdA=%d, fdB=%d\n", fd1, fd2 );

	printf ( "Writing '0123456789' and 'abcdefgh' to pipe A\n" );
	write ( fd1, "0123456789", 10 );
	write ( fd1, "abcdefgh", 8 );

	char buf[30];

	read ( fd1, buf, 15 );
	buf[15] = 0;
	printf ( "Read from A 15 char: %s\n", buf );
	printf ( "Writing it to B\n" );
	write ( fd2, buf, 15 );
	read ( fd2, buf, 10 );
	buf[10] = 0;
	printf ( "Read from B 10 char: %s\n", buf );
	write ( fd1, buf, 10 );
	printf ( "Writing it to A\n" );

	memset ( buf, 0, 30 );
	read ( fd1, buf, 15 );
	printf ( "Read from A (all): %s\n", buf );
	memset ( buf, 0, 30 );
	read ( fd2, buf, 15 );
	printf ( "Read from B (all): %s\n", buf );

    printf ( "Closing pipe A... \n");
    delete_pipe ("A");
    printf ( "Closing pipe B... \n");
    delete_pipe ("B");

    printf ( "Trying to write A again...\n");
    printf ( "Writing '0123456789' and 'abcdefgh' to pipe A\n" );
    write ( fd1, "0123456789", 10 );
    write ( fd1, "abcdefgh", 8 );
    printf ( "Reading from A again...\n" );
    read ( fd1, buf, 15 );
    printf ( "Read from A (all): %s\n", buf );

#if 0	/* test escape sequence */
	printf ( "\x1b[20;40H" "Hello World at 40, 20!\n" );
#endif

	return 0;
}
