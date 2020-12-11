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

    // initlize type with max - 1 ( 1 highest first )
    int msg_type=1;

    queue_id = msgget(KEY, 0);

    while(1){


        int rc = msgrcv(queue_id, &msg, 101, msg_type, IPC_NOWAIT) ;

        if ( rc < 0) {
            // this will be reached if there is no msgs left of the same type ( msg_type )
            msg_type++;
            if (msg_type > 10){
                // got them all
                exit(0);
            }
            // continue if there is no msgs left of this type and increase type
            continue;
        }

        // print msg
        printf("RECEVER : \t Reader '%d' read message: '%s'\n", msg_type, msg.mtext);

    }

}
