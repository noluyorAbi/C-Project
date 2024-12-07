#include "shared_memory.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int createSharedMemory(int numPlayers) {
  size_t shmSize = sizeof(SharedMemory) + numPlayers * sizeof(Player);
  int shmid = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0666);
  if (shmid == -1) {
    fprintf(stderr, "shmget failed: %s\n", strerror(errno));
    return -1;
  }
  return shmid;
}

SharedMemory *attachSharedMemory(int shmid) {
  void *shmaddr = shmat(shmid, NULL, 0);
  if (shmaddr == (void *) -1) {
    fprintf(stderr, "shmat failed: %s\n", strerror(errno));
    return NULL;
  }
  return (SharedMemory *) shmaddr;
}

int detachSharedMemory(SharedMemory *shm) {
  if (shmdt((void *) shm) == -1) {
    fprintf(stderr, "shmdt failed: %s\n", strerror(errno));
    return -1;
  }
  return 0;
}

int removeSharedMemory(int shmid) {
  if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    fprintf(stderr, "shmctl(IPC_RMID) failed: %s\n", strerror(errno));
    return -1;
  }
  return 0;
}