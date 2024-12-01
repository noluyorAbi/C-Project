#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MOCK_GAME_DATA "MockGameState"

/**
 * Sends a signal to the Thinker process to notify it of new data.
 * 
 * @param thinker_pid The process ID of the Thinker process.
 */
void send_signal_to_thinker(pid_t thinker_pid) {
  if (kill(thinker_pid, SIGUSR1) == -1) {
    perror("Connector: Failed to send signal to Thinker");
    exit(EXIT_FAILURE);
  }

  printf("Connector: Signal sent to Thinker (PID: %d)\n", thinker_pid);
}

/**
 * Main function for the Connector process.
 * - Writes mock game data to a named pipe.
 * - Sends a signal to the Thinker process to process the data.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments. Expects Thinker PID as the first argument.
 * @return int Exit status of the program.
 */
int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <Thinker PID>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  pid_t thinker_pid = atoi(argv[1]);

  if (mkfifo("/tmp/thinker_pipe", 0666) == -1 && errno != EEXIST) {
    perror("Connector: Failed to create named pipe");
    exit(EXIT_FAILURE);
  }

  int pipe_fd = open("/tmp/thinker_pipe", O_WRONLY);
  if (pipe_fd == -1) {
    perror("Connector: Failed to open pipe");
    exit(EXIT_FAILURE);
  }

  printf("Connector: Writing mock game data to the pipe...\n");
  if (write(pipe_fd, MOCK_GAME_DATA, strlen(MOCK_GAME_DATA)) == -1) {
    perror("Connector: Failed to write to pipe");
    exit(EXIT_FAILURE);
  }
  close(pipe_fd);

  send_signal_to_thinker(thinker_pid);

  printf("Connector: Denkanstoß erfolgreich gegeben!\n");
  return 0;
}
