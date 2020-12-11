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

bool chopsticks_shared[5]= { false };

bool think(int id, bool *chopsticks, sem_t *sem);

void eat(bool *chopsticks, int id, sem_t *sem);


void main(int argc, char* argv[]) {



    int id;
    #define SNAME "/HamdanSem"
    int KEY = 9009;
    int SEGMENTSIZE = sizeof(chopsticks_shared);

    id = shmget(KEY, SEGMENTSIZE, IPC_CREAT | 0666);

    if (id < 0) {
        printf("pline: shmget fdailed: ");
        exit(1);
    }

    // shared object in SHM between processes
    bool *chopsticks = (bool *) shmat(id, NULL, 0);

    if (chopsticks <= (bool * )(0)) {
        printf("pline: shmat failed: ");
        exit(2);
    }

//    printf("print\n");
//    for (int i = 0 ; i < 5 ; i ++){
//        printf("%d ",chopsticks[i]);
//    }
//    printf("\n");

     /* initialize semaphores */

    /*
        Initial value is 1.
        there is 5 chopsticks , so two philosophers can eat at the same time !
        but they can check one by one
    */
    sem_t *sem = sem_open(SNAME, O_CREAT, 0644, 1);

    /*
        sem_t *sem = sem_open(SNAME, 0); /* Open a preexisting semaphore.
        sem_post(sem);
        sem_wait(sem);
    */
    if (fork()==0) {
        //philosopher 0
        int id = 0;
        while(1){
            if (think(id, chopsticks, sem)){
                // CS
                eat(chopsticks, id, sem);
            }
            sleep(rand() % 5);
        }
    }

    if (fork()==0) {
        //philosopher 1
        int id = 1;
        while(1){
            if (think(id, chopsticks, sem)){
                // CS
                eat(chopsticks, id, sem);
            }
            sleep(rand() % 5);

        }
    }
    if (fork()==0) {
        //philosopher 2
        int id = 2;
        while(1){
            if (think(id, chopsticks, sem)){
                // CS
                eat(chopsticks, id, sem);
            }
            sleep(rand() % 5);
        }
    }

    if (fork()==0) {
        //philosopher 3
        int id = 3;
        while(1){
            if (think(id, chopsticks, sem)){
                // CS
                eat(chopsticks, id, sem);
            }
            sleep(rand() % 5);

        }
    }
    if (fork()==0) {
        //philosopher 4
        int id = 4;
        while(1){
            if (think(id, chopsticks, sem)){
                // CS
                eat(chopsticks, id, sem);
            }
            sleep(rand() % 5);

        }
    }

    while(1);
}


bool think(int id, bool *chopsticks, sem_t *sem){
    // wait turn to check if he can eat
    sem_wait(sem);

//    for (int i = 0 ; i < 5 ; i ++){
//        printf("%d ",chopsticks[i]);
//    }
    printf("\n");
    // if mine ( left ) and the next one ( right ) is available , take turn
    if (chopsticks[id] == false && chopsticks[(id + 1) % 5] == false ) {
        chopsticks[id]      = true;
        chopsticks[(id + 1) % 5]  = true;
//        for (int i = 0 ; i < 5 ; i ++){
//            printf("%d ",chopsticks[i]);
//        }
        printf("\n");
        sem_post(sem);
        return true;
    }
    else
    {
        printf ("Philosopher %d : can't take turn, some chopsticks are in use ! \n ", id);

        // let another try
        sem_post(sem);
        sleep(rand() % 5);
        return false;
    }

}

void eat(bool *chopsticks, int id, sem_t *sem){

    printf("Philosopher %d : I am eating ! \n", id);

    sleep(rand() % 20);
    sem_wait(sem);
    chopsticks[id]      = false;
    chopsticks[(id + 1) % 5]  = false;
    printf("Philosopher %d :Finished ! \n", id);
    sem_post(sem);

    sleep(rand() % 10);


}