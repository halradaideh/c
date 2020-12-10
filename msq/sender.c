#include <stdio.h>       
#include <stdlib.h>      
#include <string.h>
#include <sys/types.h>   
#include <sys/ipc.h>     
#include <sys/msg.h>     

struct msgbuf {
       long mtype;     
       char mtext[100]; 
};

int main(int argc, char* argv[])
{
    int queue_id;             
    struct msgbuf msg;

    queue_id = msgget(123456, IPC_CREAT | 0666);
    if (queue_id == -1) {
	perror("main: msgget");
	exit(1);
    }

    msg.mtype = atoi(argv[1]);
    strcpy(msg.mtext, argv[2]);

    if ( msgsnd(queue_id, &msg, 101, 0) < 0 ) {
        perror("main: msgsnd");
        exit(1);
    }
    
    printf("SENDER : \t msg generated %s %d, exiting.\n", msg.mtext , msg.mtype);

    return 0;
}
