#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "line.h"

main() {
  int i, id;
  struct info* ctrl;

  int KEY=1324;
  int SEGMENTSIZE=1000;

  id = shmget(KEY, SEGMENTSIZE, IPC_CREAT | 0666);

  if (id < 0) {
    printf("pline: shmget fdailed: ");
    exit(1);
  }

  ctrl = (struct info * ) shmat(id, 0, 0);
  if (ctrl <= (struct info * )(0)) {
    printf("pline: shmat failed: ");
    exit(2);
  }

  /* Set default parameters */
  ctrl -> c = 'a';
  ctrl -> length = 10;

  /* Main loop - print a line every 4 seconds */
  while (ctrl -> length > 0) {
    for (i = 0; i < ctrl -> length; i++)
      putchar(ctrl -> c);
    putchar('\n');

    usleep(1000000);
  }
  exit(0);
}
