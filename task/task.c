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

int MSQ_ID = 2642;

void gen_random(char *s, const int len) {
    static const char alphanum[] =     "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

int main(int argc, char* argv[])
{
    int queue_id;
    struct msgbuf msg;

    queue_id = msgget(MSQ_ID, IPC_CREAT | 0666);
    if (queue_id == -1) {
	perror("main: msgget");
	exit(1);
    }

//    struct msqid_ds buf;
//    int rc = msgctl(queue_id, IPC_STAT, &buf);
//
//    uint m = (uint)(buf.msg_qnum);
//    printf("# messages before post: %u\n", m);

    int number_of_messages=rand() % 100;
    while(number_of_messages != 0){
        msg.mtype = rand() ;

        char *str[99];
        gen_random(str, 99 );
        strcpy(msg.mtext, str);

        if ( msgsnd(queue_id, &msg, 101, 0) < 0 ) {
            perror("main: msgsnd");
            exit(1);
        }

        number_of_messages--;
    }

    int fds1[2];
    pipe(fds1);

    int fds2[2];
    pipe(fds2);

    if(fork()==0){
        close(fds1[0]);
        int counter=0;
        struct msgbuf ms;
        while(1){
            int rc = msgrcv(queue_id, &ms, 101, 0, IPC_NOWAIT);
            if ( rc == -1) {
                printf("Child 1 rec %d msgs\n", counter);
                break;
            }
            counter++;
        }

        write(fds1[1], &counter, sizeof(counter));

        exit(0);
    }

    if(fork()==0){
        close(fds2[0]);
        int counter=0;
        struct msgbuf ms;
        while(1){
            int rc = msgrcv(queue_id, &ms, 101, 0, IPC_NOWAIT);
            if ( rc == -1) {
                printf("Child 2 rec %d msgs\n", counter);
                break;
            }
            counter++;
        }

        write(fds2[1], &counter, sizeof(counter));

        exit(0);
    }

    close(fds1[1]);
    close(fds2[1]);

    wait(0);
    wait(0);

    int n1;
    read(fds1[0], &n1, sizeof(n1));

    int n2;
    read(fds2[0], &n2, sizeof(n2));

    printf("n1 %d , n2 %d\n", n1 , n2);

    if (n1 > n2){
        printf("n1 won\n");
    }else{
         printf("n2 won\n");
     }

    return 0;
}
