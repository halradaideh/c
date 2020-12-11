#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
        int     pipe_fd[2];
        pid_t   childpid;

        int SIZE=1024;
        char    readbuffer[80];

        pipe(pipe_fd);

        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }

        if(childpid == 0)
        {
                close(pipe_fd[0]);//Child process does not need this end of the pipe

                // read file into string ( char array )

                int in_fd = open("file.txt", O_RDONLY);
                char buffer[SIZE];
                int bytes_read = read(in_fd, buffer, SIZE);


                /* Send "string" through the output side of pipe */
                write(pipe_fd[1], buffer, bytes_read+1);

                exit(0);
        }
        else
        {
                /* Parent process closes up output side of pipe */
                close(pipe_fd[1]);//Parent process does not need this end of the pipe

                // Write file into FD ( can be read from pipe )
                int output_file_fd = open("out.txt",O_WRONLY | O_APPEND | O_CREAT, 0644 );

                char buffer[SIZE];

                /* Read in a string from the pipe */
                int bytes_read = read(pipe_fd[0], buffer, SIZE);
                int bytes_written = write(output_file_fd, buffer, bytes_read -1 ); // -1 to remove null character

                if(bytes_written == -1){                              // check for errors
                    perror("Failed to write to file");
                    exit(1);
                }

                printf("Read string: %s", buffer);
        }
        wait(0);
        return(0);
}