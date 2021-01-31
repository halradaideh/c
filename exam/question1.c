#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

// prototype for the factorial function
long factorial(int);

int main( void ) {
    // list for PIDs for each process
    int pids[4];

    // list of results for each process
    double results[4];

    // Pipes definitions
    int p1[2];
    pipe(p1);
    int p2[2];
    pipe(p2);
    int p3[2];
    pipe(p3);
    int p4[2];
    pipe(p3);

    // close upload end for parent for each pipe
    // 0 download
    // 1 upload
    close(fds[1]);
    close(fds[1]);
    close(fds[1]);
    close(fds[1]);

    // fact expt sin log

	int pid[1] = fork();
	if ( pid[1] == 0 ) {
	    // close download end
	    close(p1[0]);

        /* Define temporary variables */
        double value;
        double result;

        /* Assign the value we will calculate the factorial of */
        value = 10;

        /* Calculate the factorial of the value */
        result = factorial(value);


        write(p1[1], &result, sizeof(result));

        return 0;
	}

	int pid[2] = fork();
	if ( pid[2] == 0 ) {
	    // close download end
	    close(p2[0]);

        /* Define temporary variables */
        double value;
        double result;

        /* Assign the value we will calculate the exp of */
        value = 3.1;

        /* Calculate the exp of the value */
        result = exp(value);


        write(p2[1], &result, sizeof(result));

        return 0;
	}

	int pid[3] = fork();
	if ( pid[3] == 0 ) {
	    // close download end
	    close(p3[0]);

        /* Define temporary variables */
        double value;
        double result;

        /* Assign the value we will calculate the sin of */
        value = 3.1;

        /* Calculate the sin of the value */
        result = sin(value);


        write(p3[1], &result, sizeof(result));

        return 0;
	}

	int pid[4] = fork();
	if ( pid[4] == 0 ) {
	    // close download end
	    close(p4[0]);

        /* Define temporary variables */
        double value;
        double result;

        /* Assign the value we will calculate the log of */
        value = 1120;

        /* Calculate the log of the value */
        result = log(value);


        write(p4[1], &result, sizeof(result));

        return 0;
	}

	// read results now

	read(p1[0], &results[0], sizeof(results[0]));
	read(p2[0], &results[1], sizeof(results[1]));
	read(p3[0], &results[2], sizeof(results[2]));
	read(p4[0], &results[3], sizeof(results[3]));

    wait();
    wait();
    wait();
    wait();

	// sum results
	int i = 0, sum=0;
	for (i ; i < 4 ; i++)
	{
	    sum = sum + results[i];
	}

	// print result
	printf("The result of the summation is : %d \n", sum);


}

// Function that returns the factorial of a n number
long factorial(int n)
{
    int c;
    long result = 1;

    for (c = 1; c <= n; c++){
        result = result * c;
    }

    return result;
}