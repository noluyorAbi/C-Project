// ============================= INCLUDES ==============================
#include "./modules/args_parser/args_parser.h"
#include "./modules/args_parser/config.h"
#include "./modules/args_parser/constants.h"
#include "./modules/shared_memory/shared_memory.c"
#include "./modules/tcp_performConnection/performConnection.h"
#include "./modules/tcp_performConnection/tcp_connection.h"
#include "./modules/think/think.h"

#include <arpa/inet.h> // For ntohs()
#include <errno.h>
#include <signal.h> // For signal handling
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define INITIAL_SIZE 1024

// ========================= GLOBAL VARIABLES ==========================
int pipe_fd[2]; // Pipe file descriptors: [0] read, [1] write

int shmid; // ID of second SHM segment for the game state
char *shm; // Pointer to second SHM segment

// ======================= NEW STRUCTURES AND VARIABLES =======================

// Structure for Shared Memory with a flag
typedef struct {
  int flag; // 1 indicates that a new move should be calculated
  char game_data[INITIAL_SIZE - sizeof(int)]; // Game state data
} shm_data_t;

// Pointer to the SHM structure
shm_data_t *shm_ptr = NULL;

// Global flag for signal handling
volatile sig_atomic_t sig_received = 0;

/**
 * @brief Signal handler for SIGINT (CTRL+C).
 *        Cleans up shared memory and terminates the program.
 *
 * @param signal The signal number received.
 */
void handle_sigint(int signal) {
  if (signal == SIGINT) {
    fprintf(stdout,
            "\nSIGINT received. Cleaning up shared memory and exiting.\n");

    // Cleanup the second SHM segment
    cleanupSHM(shmid, shm);
    exit(EXIT_SUCCESS);
  }
}

/**
 * @brief Signal handler for the Thinker process.
 *        Sets the sig_received flag when SIGUSR1 is received.
 *
 * @param signal The signal number received.
 */
void handle_signal(int signal) {
  if (signal == SIGUSR1) {
    sig_received = 1;
  }
}

// ========================= FUNCTION PROTOTYPES =======================
static int initialize_game(int argc, char *argv[], GameConfig *game_config,
                           Config *app_config);
static void createBoardMemory(); // Creates second SHM segment
static int create_pipe();
static int fork_processes(GameConfig game_config, char *piece_data);
static void run_connector(GameConfig game_config, char *piece_data);
static void run_thinker(pid_t pid);

/**********************************************************
 *                       MAIN FUNCTION                    *
 *********************************************************/
int main(int argc, char *argv[]) {
  GameConfig game_config;
  Config app_config;

  // Initialize the game and configurations
  if (initialize_game(argc, argv, &game_config, &app_config) != 0) {
    return EXIT_FAILURE;
  }

  // Create second SHM segment
  createBoardMemory();

  // Cast shm to shm_data_t structure
  shm_ptr = (shm_data_t *) shm;

  // Initialize flag and game_data in SHM
  shm_ptr->flag = 0;
  memset(shm_ptr->game_data, 0, sizeof(shm_ptr->game_data));

  // Create buffer to store piece data (game state data)
  char piece_data[INITIAL_SIZE] = "";

  // Create an inter-process communication pipe
  if (create_pipe() != 0) {
    cleanupSHM(shmid, shm); // Cleanup before exiting
    return EXIT_FAILURE;
  }

  struct sigaction sa_int;

  sa_int.sa_handler = handle_sigint; // Assign the handler function
  sa_int.sa_flags = 0;               // No special flags
  sigemptyset(&sa_int.sa_mask);      // No signals blocked during handler

  if (sigaction(SIGINT, &sa_int, NULL) == -1) {
    fprintf(stderr, "Error setting up SIGINT handler: %s\n", strerror(errno));
    cleanupSHM(shmid, shm); // Cleanup before exiting
    return EXIT_FAILURE;
  }

  // Fork processes for connector and thinker logic
  return fork_processes(game_config, piece_data);
}
/**********************************************************
 *                   END OF MAIN FUNCTION                 *
 *********************************************************/

// ========================= ---- ====================== //

/**********************************************************
 *                    HELPER FUNCTIONS                    *
 *********************************************************/

/**
 * @brief Initialize the game configuration and application configuration.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @param game_config Pointer to GameConfig structure.
 * @param app_config Pointer to Config structure.
 * @return int 0 on success, -1 on failure.
 */
static int initialize_game(int argc, char *argv[], GameConfig *game_config,
                           Config *app_config) {
  if (!parse_args(argc, argv, game_config)) {
    print_usage(argv[0]);
    return -1;
  }

  printf("GAME-ID: %s\n", game_config->game_id);
  printf("Number of Players: %d\n", game_config->player_number);
  printf("Configuration File: %s\n", game_config->config_file);

  if (!parse_config_file(game_config->config_file, app_config)) {
    fprintf(stderr, "Error: Invalid configuration file. %s\n",
            game_config->config_file);
    return -1;
  }

  printf("Hostname: %s\n", app_config->hostname);
  printf("Port Number: %u\n", ntohs(app_config->portNumber));
  printf("Game Kind Name: %s\n", app_config->gameKindName);

  return 0;
}

/**
 * @brief Creates a SHM segment for the game state.
 */
static void createBoardMemory() {
  // Create second SHM segment with read and write permissions
  shmid = shmget(IPC_PRIVATE, sizeof(shm_data_t), IPC_CREAT | 0666);
  if (shmid < 0) {
    fprintf(stderr, "Shared memory creation for board failed.\n");
    exit(EXIT_FAILURE);
  } else {
    fprintf(stdout, "Shared memory creation for board was successful.\n");
  }

  // Attach SHM segment to the process's address space
  shm = (char *) shmat(shmid, NULL, 0);
  if (shm == (char *) -1) {
    fprintf(stderr, "shmat failed.\n");
    exit(EXIT_FAILURE);
  } else {
    fprintf(stdout, "shmat was successful.\n");
  }

  // Initialize the SHM structure
  shm_ptr = (shm_data_t *) shm;
  shm_ptr->flag = 0;
  memset(shm_ptr->game_data, 0, sizeof(shm_ptr->game_data));
}

/**
 * @brief Creates a pipe for inter-process communication.
 *
 * @return int 0 on success, -1 on failure.
 */
static int create_pipe() {
  if (pipe(pipe_fd) == -1) {
    fprintf(stderr, "Failed to create pipe: %s\n", strerror(errno));
    return -1;
  }
  return 0;
}

/**
 * @brief Fork processes and manage their execution.
 *
 * @param game_config The configuration for the game.
 * @param piece_data Buffer for storing game state.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
static int fork_processes(GameConfig game_config, char *piece_data) {
  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork failed: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  if (pid == 0) {
    // Child process: Run connector
    run_connector(game_config, piece_data);
    // Child process should not continue beyond run_connector
    exit(EXIT_SUCCESS);
  } else {
    // Parent process: Run thinker
    run_thinker(pid);
  }

  return EXIT_SUCCESS;
}

/**
 * @brief Run the connector process logic.
 *
 * @param game_config The configuration for the game.
 * @param piece_data Buffer for storing game state.
 */
static void run_connector(GameConfig game_config, char *piece_data) {
  // Close the write end of the pipe in the connector process
  if (close(pipe_fd[1]) == -1) {
    fprintf(stderr, "Connector: Error closing write end of the pipe: %s\n",
            strerror(errno));
  }

  // Establish connection
  if (createConnection(game_config.game_id, piece_data) != 0) {
    fprintf(stderr, "Connector: Failed to establish connection.\n");
    // Close the read end of the pipe only if createConnection fails
    if (close(pipe_fd[0]) == -1) {
      fprintf(stderr, "Connector: Error closing read end of the pipe: %s\n",
              strerror(errno));
    }
    exit(EXIT_FAILURE);
  }

  // Set the flag and populate game_data after successful connection
  shm_ptr->flag = 1;
  strncpy(shm_ptr->game_data, "Current game state data...",
          sizeof(shm_ptr->game_data) - 1);
  shm_ptr->game_data[sizeof(shm_ptr->game_data) - 1] =
    '\0'; // Ensure null-termination

  // Send SIGUSR1 signal to the Thinker process (Parent)
  if (kill(getppid(), SIGUSR1) == -1) {
    fprintf(stderr, "Connector: Failed to send SIGUSR1 to Thinker: %s\n",
            strerror(errno));
  }

  // Close the read end of the pipe immediately after successful connection
  if (close(pipe_fd[0]) == -1) {
    fprintf(stderr, "Connector: Error closing read end of the pipe: %s\n",
            strerror(errno));
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

/**
 * @brief Run the thinker process logic.
 *
 * @param pid The process ID of the connector process.
 */
static void run_thinker(pid_t pid) {
  // Close the read end of the pipe in the thinker process
  if (close(pipe_fd[0]) == -1) {
    fprintf(stderr, "Thinker: Error closing read end of the pipe: %s\n",
            strerror(errno));
  }

  // Set up the signal handler for SIGUSR1
  struct sigaction sa;
  sa.sa_handler = handle_signal; // Assign the handler function
  sa.sa_flags = 0;               // No special flags
  sigemptyset(&sa.sa_mask);      // No signals blocked during handler
  if (sigaction(SIGUSR1, &sa, NULL) == -1) {
    fprintf(stderr, "Thinker: sigaction failed.\n");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "Thinker: Ready and waiting for signals...\n");

  // Main loop to wait for signals
  while (1) {
    pause(); // Wait for any signal

    if (sig_received) {
      sig_received = 0; // Reset the flag

      // Check if the SHM flag is set
      if (shm_ptr->flag) {
        // Make a move
        think(shm_ptr->game_data);
        // Reset the SHM flag after processing
        shm_ptr->flag = 0;
      }
    }
  }

  // TODO: Break loop when server sends GAMEOVER or QUIT
  // Note: The code below is unreachable due to the infinite loop,
  // but it's good practice to include cleanup in case the loop exits.

  // Wait for connector to terminate
  int status;
  if (waitpid(pid, &status, 0) == -1) {
    fprintf(stderr, "Thinker: Error waiting for child process: %s\n",
            strerror(errno));
    if (close(pipe_fd[1]) == -1) {
      fprintf(stderr, "Thinker: Error closing write end of the pipe: %s\n",
              strerror(errno));
    }
    cleanupSHM(shmid, shm);
    exit(EXIT_FAILURE); // Exit thinker if waitpid fails
  }

  if (WIFEXITED(status)) {
    fprintf(stdout, "Connector exited with status %d.\n", WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    fprintf(stdout, "Connector terminated by signal %d.\n", WTERMSIG(status));
  }

  // Close the write end of the pipe in the thinker process
  if (close(pipe_fd[1]) == -1) {
    fprintf(stderr, "Thinker: Error closing write end of the pipe: %s\n",
            strerror(errno));
  }

  // Cleanup the second SHM segment
  cleanupSHM(shmid, shm);
}
/**********************************************************
 *                END OF HELPER FUNCTIONS                 *
 *********************************************************/