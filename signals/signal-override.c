#include <stdio.h>
#include <signal.h>

void func() {
  printf("Hi\n");
}

main() {
  signal(SIGUSR1, func);
  while (1);
}
