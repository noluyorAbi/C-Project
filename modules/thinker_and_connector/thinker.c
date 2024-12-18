// thinker.c

#include "thinker.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MOVE "A1\n" // Simulated move

int pipe_fd; // Global pipe file descriptor for writing

/**
 * Signal handler for SIGUSR1. Activates the Thinker to generate a move.
 */
void sigusr1_handler(int signum) {
  printf("Thinker: Received SIGUSR1 signal.\n");

  // Simulate generating a move
  const char *move = MOVE;
  printf("Thinker: Generating move: %s", move);

  // Write the move to the pipe
  if (write(pipe_fd, move, strlen(move)) == -1) {
    perror("Thinker: Failed to write move to pipe");
    exit(EXIT_FAILURE);
  }

  printf("Thinker: Move sent to Connector via pipe.\n");
}

int main() {
  // Close unused read end of the pipe
  close(pipe_fd);

  // Set up signal handler for SIGUSR1
  struct sigaction sa;
  sa.sa_handler = sigusr1_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGUSR1, &sa, NULL) == -1) {
    perror("Thinker: Failed to set up SIGUSR1 handler");
    exit(EXIT_FAILURE);
  }

  printf("Thinker: Waiting for signals...\n");
  while (1) {
    pause(); // Wait for SIGUSR1 signal
  }

  return 0;
}