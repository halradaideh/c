#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "line.h"

main(int argc, char* argv[]) {
  int id;
  struct info* ctrl;

  if (argc != 3)  {
    fprintf(stderr, "usage: b <char> <length>\n");
    exit(3);
  }

  int KEY = 1324;
  int SEGMENTSIZE = 1000;

  id = shmget(KEY, SEGMENTSIZE, 0);

  if (id < 0) {
    printf("cline: shmget failed: ");
    exit(1);
  }

  ctrl = (struct info * ) shmat(id, 0, 0);
  if (ctrl <= (struct info * )(0))  {
    printf("cline: shmat failed: ");
    exit(2);
  }

  /* Copy command line data to shared memory region */
  ctrl -> c = argv[1][0];
  ctrl -> length = atoi(argv[2]);
  exit(0);
}