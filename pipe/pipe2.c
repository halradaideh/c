#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main()
{
        int     filedes[2], nbytes;
        pid_t   childpid;
        char    string[] = "Hello, world!\n";
        char    readbuffer[80];

        pipe(filedes);

        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }

        if(childpid == 0)
        {
                close(filedes[0]);//Child process does not need this end of the pipe

                /* Send "string" through the output side of pipe */
                write(filedes[1], string, (strlen(string)+1));
                while(1);
                exit(0);
        }
        else
        {
                /* Parent process closes up output side of pipe */
                close(filedes[1]);//Parent process does not need this end of the pipe

                /* Read in a string from the pipe */
                nbytes = read(filedes[0], readbuffer, (strlen(string)+1));
                printf("Read string: %s", readbuffer);
        }
        wait(0);
        return(0);
}