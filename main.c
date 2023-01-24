#include<stdio.h>
#include<stdlib.h>

#include "dsm.h"

int main(int arg, char *argv[])
{
  // Check whether it's a master or client node
  int master = strcmp(argv[1], "master") == 0;
  // Get master & client ip
  char *masterip = argv[2];
  char *clientip = argv[3];

  printf("master=%d, masterip=%s, clientip=%s\n", master, masterip, clientip);

  // Initialize distributed shared memory
  initializeDSM(master, masterip, 20000, clientip, 20001, 10000);

  // Get the shared region memory
  void *sharedregion = getsharedregion();

  if (master)
  {
    // Shared region start point address
    int *p1 = (int *) sharedregion;

    // Shared region start point address + 1
    int *p2 = ((int *) sharedregion) + 1;

    for (int i = 0; 1; i++)
    {
      // Update p1 value from master to test whether client nodes notice
      *p1 = i;

      // Wait for 500ms
      usleep(500000);

      printf("p2: %ld\n", *p2);
    }
  }
  else
  {
    while (1)
    {
      // Shared region start point address
      int *p1 = (int *) sharedregion;

      // Shared region start point address + 1
      int *p2 = ((int *) sharedregion) + 1;

      // Wait for 500ms
      usleep(500000);

      printf("p1: %ld\n", *p1);

      // Update p2 value from client to test whether the master node notice
      *p2 += 5;
    }
  }

  destrongDSM();

  return 0;
}
