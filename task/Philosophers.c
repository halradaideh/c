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

bool think(int id, bool *chopsticks, sem_t *sem);

void eat(bool *chopsticks, int id, sem_t *sem);

void cleaner();

int id;
#define SNAME "/HamdanSem1"
bool chopsticks_shared[5]= { false };
int KEY = 90109;
sem_t *sem;
bool *chopsticks;

int pid0, pid1, pid2, pid3, pid4  = -1;

void main(int argc, char* argv[]) {
    int SEGMENTSIZE = sizeof(chopsticks_shared);

    id = shmget(KEY, SEGMENTSIZE, IPC_CREAT | 0666);

    if (id < 0) {
        printf("pline: shmget fdailed: ");
        exit(1);
    }

    // shared object in SHM between processes
    chopsticks = (bool *) shmat(id, NULL, 0);

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
    sem = sem_open(SNAME, O_CREAT, 0644, 1);

    /*
        sem_t *sem = sem_open(SNAME, 0); /* Open a preexisting semaphore.
        sem_post(sem);
        sem_wait(sem);
    */

    pid0=fork();
    pid1=fork();
    pid2=fork();
    pid3=fork();
    pid4=fork();

    if ( pid0 == 0 && pid1 != 0 && pid2 != 0 && pid3 != 0 && pid4 != 0 ) {
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
    if (pid0 != 0 && pid1 == 0 && pid2 != 0 && pid3 != 0 && pid4 != 0) {
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
    if (pid0 != 0 && pid1 != 0 && pid2 == 0 && pid3 != 0 && pid4 != 0) {
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
    if (pid0 != 0 && pid1 != 0 && pid2 != 0 && pid3 == 0 && pid4 != 0) {
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
    if (pid0 != 0 && pid1 != 0 && pid2 != 0 && pid3 != 0 && pid4 == 0) {
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
    if (pid0 != 0 && pid1 != 0 && pid2 != 0 && pid3 != 0 && pid4 != 0)
    {
        signal(SIGINT, cleaner);
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

void cleaner(){
    // kill children
    printf("\n[-] Kill Children \n");
    printf("%d\n",pid0);
    kill(pid0,SIGKILL);
    printf("%d\n",pid1);
    kill(pid1,SIGKILL);
    printf("%d\n",pid2);
    kill(pid2,SIGKILL);
    printf("%d\n",pid3);
    kill(pid3,SIGKILL);
    printf("%d\n",pid4);
    kill(pid4,SIGKILL);

    // reset SHM
    printf("[-] reset SHM \n");
    for(int i = 0 ; i < 5 ; i++){
        chopsticks[i] = false;
    }

    // reset Sem
    printf("[-] reset Sem \n");
    int value = -1;
    sem_getvalue(sem, &value);
    printf("Current Sem value %d\n", value);
    while(value != 1){
        if(value > 1){
            sem_wait(sem);
        }
        else{
            sem_post(sem);
        }
        sem_getvalue(sem, &value);
    }
    printf("New Sem value %d\n", value);

    // exit
    printf("[-] Exit with status code 0 \n");
    exit(0);

}