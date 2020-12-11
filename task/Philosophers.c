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
#include <pthread.h>

bool chopsticks[5]= { false };

bool think(int id, bool *chopsticks, sem_t *sem);

void eat(bool *chopsticks, int id, sem_t *sem);


void main(int argc, char* argv[]) {



    int id;
    #define SNAME "/mjyf"
    int KEY = 42;
    int SEGMENTSIZE = sizeof(chopsticks);

    id = shmget(KEY, SEGMENTSIZE, IPC_CREAT | 0666);

    if (id < 0) {
        printf("pline: shmget fdailed: ");
        exit(1);
    }

    // shared object in SHM between processes
    chopsticks[5] = (bool *) shmat(id, NULL, 0);

     /* initialize semaphores */

    /*
        Initial value is 1.
        there is 5 chopsticks , so two philosophers can eat at the same time !
        but they can check one by one
    */
    sem_t *sem = sem_open(SNAME, O_CREAT, 0644, 2);

    /*
        sem_t *sem = sem_open(SNAME, 0); /* Open a preexisting semaphore.
        sem_post(sem);
        sem_wait(sem);
    */
    int id1, id2, id3, id4, id0;
    if (id0=fork()==0) {
        //philosopher 1
        int id = 1;
        while(1){
            if (think(id, chopsticks, sem)){
                // CS
                eat(chopsticks, id, sem);
            }
            sleep(1);
        }
    }

    if (id1=fork()==0) {
        //philosopher 3
        int id = 3;
        while(1){
            if (think(id, chopsticks, sem)){
                // CS
                eat(chopsticks, id, sem);
            }
            sleep(1);

        }
    }
    if (id2=fork()==0) {
        //philosopher 2
        int id = 2;
        while(1){
            if (think(id, chopsticks, sem)){
                // CS
                eat(chopsticks, id, sem);
            }
            sleep(1);

        }
    }
    if (id3=fork()==0) {
        //philosopher 4
        int id = 4;
        while(1){
            if (think(id, chopsticks, sem)){
                // CS
                eat(chopsticks, id, sem);
            }
            sleep(1);

        }
    }
    if (id4=fork()==0) {
        //philosopher 5
        int id = 5;
        while(1){
            if (think(id, chopsticks, sem)){
                // CS
                eat(chopsticks, id, sem);
            }
            sleep(1);

        }
    }
    int value = -1;
    int prev = -100;
    while(1)
    {
        usleep(1000);
        prev = value;
        sem_getvalue(sem, &value);
        if (value != prev){
            printf("*** eating now: %d ***\n", abs(value- 2));
        }
    }
}


bool think(int id, bool *chopsticks, sem_t *sem){
    sleep(rand() % 5 );
    // wait turn to check if he can eat
    sem_wait(sem);

    // to match index
    id -- ;

    // if mine ( left ) and the next one ( right ) is available , take turn
    if (chopsticks[id] == false && chopsticks[id % 5] == false ) {
        chopsticks[id]      = true;
        chopsticks[id % 5]  = true;

        return true;
    }
    else
    {
        printf ("Philosopher %d : can't take turn, some chopsticks are in use ! \n ", id);

        // let another try
        sem_post(sem);
        sleep(1);
        return false;
    }

}

void eat(bool *chopsticks, int id, sem_t *sem){

    printf("Philosopher %d : I am eating ! \n", id);
    sleep(rand() % 2);
    
    printf("\tPhilosopher %d : Finished eating ! \n", id);

    id --;
    chopsticks[id]      = false;
    chopsticks[id % 5]  = false;
    sem_post(sem);

}