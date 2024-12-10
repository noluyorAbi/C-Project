#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256

/**
 * Signal handler for the Thinker process.
 * - Reads game data from a named pipe.
 * - Simulates calculation of the next move.
 *
 * @param signal The signal received (expected SIGUSR1).
 */
void handle_signal(int signal) {
  if (signal == SIGUSR1) {
    printf("Thinker: Signal received. Processing data...\n");

    int pipe_fd = open("/tmp/thinker_pipe", O_RDONLY);
    if (pipe_fd == -1) {
      perror("Thinker: Failed to open pipe");
      exit(EXIT_FAILURE);
    }

    char game_data[BUFFER_SIZE] = {0};
    if (read(pipe_fd, game_data, BUFFER_SIZE) == -1) {
      perror("Thinker: Failed to read from pipe");
      close(pipe_fd);
      exit(EXIT_FAILURE);
    }
    close(pipe_fd);

    printf("Thinker: Received game data: %s\n", game_data);

    sleep(2);
    printf("Thinker: Calculated move: A1:B4\n");
  }
}

/**
 * Main function for the Thinker process.
 * - Sets up a signal handler for SIGUSR1.
 * - Waits for signals in a loop.
 *
 * @return int Exit status of the program.
 */
int main() {
  struct sigaction sa;
  sa.sa_handler = handle_signal;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  if (sigaction(SIGUSR1, &sa, NULL) == -1) {
    perror("Thinker: sigaction failed");
    exit(EXIT_FAILURE);
  }

  printf("Thinker: Ready and waiting for signals...\n");

  while (1) {
    pause();
  }

  return 0;
}
