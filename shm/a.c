#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    long mtype;
    char mtext[100];
} info ;

void main(int argc, char* argv[]) {
    int id;
    int KEY = 1243;
    int SEGMENTSIZE = sizeof(info);

    id = shmget(KEY, SEGMENTSIZE * 5, IPC_CREAT | 0666);

    if (id < 0) {
        printf("pline: shmget fdailed: ");
        exit(1);
    }

    info *msgs = (info *) shmat(id, NULL, 0);

    for (int j = 0; j < 5; j++) {
        strcpy(msgs[j].mtext, "new");
        msgs[j].mtype = j + 1;
    }
    printf("init.\n");
    while (1) {
        printf("\nnew while \n");
        for (int j = 0; j < 5; j++) {
            printf("MGS %d : type %d , msg %s\n", j + 1 , msgs[j].mtype, msgs[j].mtext);
        }
        usleep(10000000);
    }

    exit(0);
}