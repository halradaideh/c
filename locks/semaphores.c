#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>

void main(int argc, char* argv[]) {

     /* initialize semaphores */
    #define SNAME "/mysem"
    sem_t *sem = sem_open(SNAME, O_CREAT, 0644, 0); /* Initial value is 3. */


    // first child
    if (fork() == 0) {
        sem_t *sem = sem_open(SNAME, 0); /* Open a preexisting semaphore. */
        printf("Child : waiting Parent post \n");
        int value = -1;
        sem_getvalue(sem, &value);
        printf("Child : sem value %d\n", value);
        sem_wait(sem);
        printf("Child : finished, signal (post) \n");
        sem_getvalue(sem, &value);
        printf("Child : sem value %d\n", value);
    } else {
        sem_t *sem = sem_open(SNAME, 0); /* Open a preexisting semaphore. */
        sleep(3);
        int value = -1;
        sem_getvalue(sem, &value);
        printf("Parent : sem value %d\n", value);
        printf("Parent : POST \n");
        sem_post(sem);
        sem_getvalue(sem, &value);
        printf("Parent : sem value %d\n", value);
        wait(0);

        sem_close(sem);
    }
    exit(0);
}