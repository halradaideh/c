// C program to show thread functions

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* func(void* arg)
{
    // detach the current thread
    // from the calling thread
    // pthread_detach(pthread_self());

    printf("Inside the thread\n");
    sleep(10);
    printf("Inside the thread\n");

    // exit the current thread
    // exit(0);
    pthread_exit(NULL);
}

void fun()
{
    pthread_t ptid;

    printf("MASTER before\n");

    // Creating a new thread
    pthread_create(&ptid, NULL, &func, NULL);

    sleep(1);

    exit(0);
    // Waiting for the created thread to terminate
    pthread_join(ptid, NULL);

    printf("MASTER after \n");

    pthread_exit(NULL);
}

// Driver code
int main()
{
    fun();
    return 0;
}
