#include <stdio.h>	// for printf()

int exec_syscall_17( int *num )
{
	int	retval = -1;
	
	asm(" 	movl	$17, %eax	");
	asm("	movl	%0, %%ebx " : : "m" (num) );
	asm("	int	$0x80		");
	asm("	movl	%%eax, %0 " : "=m" (retval) );
	
	return	retval;
}	
	

int main( int argc, char **argv )
{
	int	number = 0;

	printf( "\nDemonstrating system-call 17 ... \nIt will count from 0 -> 5 inside the Kernel\n" );

	for (int i = 0; i <= 5; i++) {
        printf( "\n#%d:  number=%d ", i, number );
        exec_syscall_17( &number );
    }

	printf( "\n\n" );
}
