#include "thinker.h"

#include "../shared_memory/shared_memory.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

static SharedMemory *shm = NULL;
volatile sig_atomic_t think_flag = 0;

void think(int pipe_fd) {
  if (shm == NULL) {
    fprintf(stderr, "[THINKER] Shared memory not attached. Exiting.\n");
    return;
  }

  if (shm->readyFlag == 1) {
    printf("[THINKER] Processing game state: %s\n", shm->gameState);
    shm->readyFlag = 0;

    const char *move = "MOVE A1\n";
    write(pipe_fd, move, strlen(move));
    printf("[THINKER] Move written to pipe: %s\n", move);
  } else {
    printf("[THINKER] No new game state to process.\n");
  }
}

void signal_handler(int signum) {
  if (signum == SIGUSR1) {
    think_flag = 1;
  }
}

void signalCleanup(int signum) {
  fprintf(stderr, "[THINKER] Caught signal %d. Cleaning up and exiting...\n",
          signum);
  if (shm != NULL) {
    detachSharedMemory(shm);
  }
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "[THINKER] Usage: %s <pipe_write_fd>\n", argv[0]);
    return EXIT_FAILURE;
  }

  int pipe_fd = atoi(argv[1]);

  int shmid = shmget(SHM_KEY, sizeof(SharedMemory), 0666);
  if (shmid == -1) {
    perror("[THINKER] Failed to retrieve shared memory ID");
    return EXIT_FAILURE;
  }

  shm = attachSharedMemory(shmid);
  if (shm == NULL) {
    fprintf(stderr, "[THINKER] Failed to attach to shared memory.\n");
    return EXIT_FAILURE;
  }

  signal(SIGUSR1, signal_handler);
  signal(SIGTERM, signalCleanup);
  signal(SIGINT, signalCleanup);

  printf("[THINKER] Ready and waiting for SIGUSR1 signals...\n");

  while (1) {
    pause();
    if (think_flag) {
      think(pipe_fd);
      think_flag = 0;
    }
  }

  // Cleanup before exiting (if the loop is broken)
  detachSharedMemory(shm);
  return EXIT_SUCCESS;
}