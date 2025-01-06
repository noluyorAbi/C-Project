#include "connector.h"

#include "../shared_memory/shared_memory.h"
#include "../tcp_performConnection/performConnection.h"
#include "thinker.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <unistd.h>

static SharedMemory *shm = NULL;
static int shmid = -1;

void signalCleanup(int signum) {
  fprintf(stderr, "[CONNECTOR] Caught signal %d. Cleaning up and exiting...\n",
          signum);
  if (shm != NULL) {
    cleanupSharedMemory(shmid, shm);
  }
  exit(EXIT_SUCCESS);
}

void run_connector(int pipe_fd, int sockfd) {
  signal(SIGTERM, signalCleanup);
  signal(SIGINT, signalCleanup);

  shmid = shmget(SHM_KEY, sizeof(SharedMemory), 0666);
  if (shmid == -1) {
    perror("[CONNECTOR] Failed to retrieve shared memory ID");
    return;
  }

  shm = attachSharedMemory(shmid);
  if (shm == NULL) {
    fprintf(stderr, "[CONNECTOR] Failed to attach to shared memory.\n");
    return;
  }

  while (1) {
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
      buffer[bytes_received] = '\0';
      printf("[CONNECTOR] Received data: %s\n", buffer);

      strncpy(shm->gameState, buffer, sizeof(shm->gameState) - 1);
      shm->readyFlag = 1;

      kill(shm->thinkerPID, SIGUSR1);
    } else if (bytes_received == 0) {
      printf("[CONNECTOR] Connection closed by server.\n");
      break;
    } else {
      perror("[CONNECTOR] Error receiving data");
    }
  }

  cleanupSharedMemory(shmid, shm);
}