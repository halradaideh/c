#include <stdio.h>
#include <pthread.h>
#include <signal.h>

#define NTHREADS 10
void *thread_function(void *);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;
long id=-1;

main()
{
   pthread_t thread_id[NTHREADS];
   int i, j;

   for(i=0; i < NTHREADS; i++)
   {
      pthread_create( &thread_id[i], NULL, thread_function, NULL );
   }

   long prev = -2;
   while (prev != id) {
    prev = id;
    sleep(4);
    if (id == prev) {
        printf("MASTER\t:\tkilling thread %ld\n", id);
        pthread_kill(id, SIGTERM);
    }
    printf("MASTER\t:\tThread %ld finished\n", id);
    sleep(1);
   }

    printf("MASTER\t:\tnow Joining \n");
   for(j=0; j < NTHREADS; j++)
   {
      pthread_join( thread_id[j], NULL);
   }

   /* Now that all threads are complete I can print the final result.     */
   /* Without the join I could be printing a value before all the threads */
   /* have been completed.                                                */

   printf("MASTER\t:\tFinal counter value: %d\n", counter);
}

void term() {
  pthread_mutex_unlock( &mutex1 );
  pthread_exit(NULL);
}

void *thread_function(void *dummyPtr)
{
   signal(SIGTERM, term);
   int slp = rand() % 15;
   pthread_mutex_lock( &mutex1 );
   // CS
   printf("\nThread\t:\t%ld\t sleeping %d\n", pthread_self(), slp);
   id = pthread_self();
   sleep(slp);
   counter++;
   // END OF CS
   pthread_mutex_unlock( &mutex1 );
}
