#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_NAME "/game_state_shm"
#define SHM_SIZE 1024

typedef struct {
  char game_state[SHM_SIZE];
  int new_data_flag; // Flag to indicate new data
} SharedMemory;

/**
 * Processes the game state from shared memory.
 */
void process_game_state(SharedMemory *shm_ptr) {
  if (shm_ptr->new_data_flag) {
    printf("Thinker: Processing game state...\n");
    printf("Game State: %s\n", shm_ptr->game_state);
    shm_ptr->new_data_flag = 0; // Reset the flag
  } else {
    printf("Thinker: No new game state to process.\n");
  }
}

/**
 * Signal handler for SIGUSR1.
 */
void sigusr1_handler(int signum) {
  printf("Thinker: Received SIGUSR1 signal.\n");

  // Open shared memory
  int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
  if (shm_fd == -1) {
    perror("Thinker: Failed to open shared memory");
    exit(EXIT_FAILURE);
  }

  SharedMemory *shm_ptr = mmap(NULL, sizeof(SharedMemory),
                               PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED) {
    perror("Thinker: Failed to map shared memory");
    exit(EXIT_FAILURE);
  }

  // Process the game state
  process_game_state(shm_ptr);

  // Clean up
  if (munmap(shm_ptr, sizeof(SharedMemory)) == -1) {
    perror("Thinker: Failed to unmap shared memory");
  }
}

int main() {
  // Set up signal handler
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
    pause(); // Wait for signals
  }

  return 0;
}