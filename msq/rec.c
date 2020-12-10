#include <stdio.h>       
#include <stdlib.h>      
#include <unistd.h>      
#include <sys/types.h>   
#include <sys/ipc.h>     
#include <sys/msg.h>     

struct msgbuf {
       long mtype;     
       char mtext[100]; 
};

void main(int argc, char* argv[])
{
    int queue_id;         
    struct msgbuf msg;   
    int msg_type;         

    msg_type = atoi(argv[1]);

    queue_id = msgget(123456, 0);

    if ( msgrcv(queue_id, &msg, 101, msg_type, 0) < 0) {
	    printf("Error rec\n");
	    exit(1);
    }

    printf("RECEVER : \t Reader '%d' read message: '%s'\n", msg_type, msg.mtext);
}
