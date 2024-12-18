#include "performConnection.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define MOCK_GAME_DATA "MockGameState"
#define SHM_NAME "/game_state_shm"
#define SHM_SIZE 1024

typedef struct {
  char game_state[SHM_SIZE];
  int new_data_flag; // Flag to indicate new data
} SharedMemory;

/**
 * Sends a THINKING command to the server.
 *
 * @param sockfd The socket file descriptor.
 */
void send_thinking_command(int sockfd) {
  const char *thinking_cmd = "THINKING\n";
  if (sendMessage(sockfd, thinking_cmd) != EXIT_SUCCESS) {
    fprintf(stderr, "Connector: Failed to send THINKING command.\n");
    exit(EXIT_FAILURE);
  }
}

/**
 * Writes game state to shared memory and sets the new data flag.
 *
 * @param shm_ptr Pointer to shared memory structure.
 * @param game_state The new game state to write.
 */
void write_to_shared_memory(SharedMemory *shm_ptr, const char *game_state) {
  strncpy(shm_ptr->game_state, game_state, SHM_SIZE);
  shm_ptr->new_data_flag = 1; // Set the new data flag
  printf("Connector: Game state written to shared memory.\n");
}

/**
 * Main function for the Connector process.
 */
int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <Thinker PID> <Server Socket FD>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  pid_t thinker_pid = atoi(argv[1]);
  int sockfd = atoi(argv[2]);

  // Establish shared memory
  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1) {
    perror("Connector: Failed to create shared memory");
    exit(EXIT_FAILURE);
  }
  if (ftruncate(shm_fd, sizeof(SharedMemory)) == -1) {
    perror("Connector: Failed to set shared memory size");
    exit(EXIT_FAILURE);
  }
  SharedMemory *shm_ptr = mmap(NULL, sizeof(SharedMemory),
                               PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED) {
    perror("Connector: Failed to map shared memory");
    exit(EXIT_FAILURE);
  }

  // Simulate receiving game state from server
  const char *new_game_state = "ServerGameState";
  write_to_shared_memory(shm_ptr, new_game_state);

  // Send THINKING command
  send_thinking_command(sockfd);

  // Notify the Thinker
  send_signal_to_thinker(thinker_pid);

  printf("Connector: THINKING command sent, signal sent to Thinker.\n");
  return 0;
}