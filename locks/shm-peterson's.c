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

static int PN = 2;

typedef struct {
    bool processes[2];
    int turn;
} shared ;

void main(int argc, char* argv[]) {
    int id;
    int KEY = 1222;
    int SEGMENTSIZE = sizeof(shared);

    id = shmget(KEY, SEGMENTSIZE, IPC_CREAT | 0666);

    if (id < 0) {
        printf("pline: shmget fdailed: ");
        exit(1);
    }

    shared *ctrl = (shared *) shmat(id, NULL, 0);

    for (int j = 0; j < PN; j++) {
        ctrl -> processes[j] = false;
    }
    ctrl -> turn = -1;
    printf("init.\n");



    // first child
    if (fork() == 0) {
        while(1){
            int id = 0;
            ctrl -> processes[id] = true;
            ctrl -> turn = 1;
            while (ctrl->turn == 1 && ctrl->processes[1]){
                 sleep(1);
            }
            printf("\n");
            printf("*******************\n");
            printf("**\tCS 1\t**\n");
            printf("*******************\n");
            printf("\n");

            ctrl -> processes[id] = false;
        }
    } else

    // second child
    if (fork() == 0) {
    while(1){
            int id = 1;
            ctrl -> processes[id] = true;
            ctrl -> turn = 2;
            while (ctrl->turn == 2 && ctrl->processes[0]){
                sleep(1);
            }
            printf("\n");
            printf("*******************\n");
            printf("**\tCS 2\t**\n");
            printf("*******************\n");
            printf("\n");
            ctrl -> processes[id] = false;
        }
    }

    wait(0);
    wait(0);
}