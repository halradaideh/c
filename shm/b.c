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

    void *shared_memory = shmat(id, NULL, 0);

    info *msgs = (info *) shared_memory;

    strcpy(msgs[atoi(argv[1])].mtext, argv[3]);
    msgs[atoi(argv[1])].mtype = atoi(argv[2]);

    exit(0);
}