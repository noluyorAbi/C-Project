// ============================= INCLUDES ==============================
#include "./modules/args_parser/args_parser.h"
#include "./modules/args_parser/config.h"
#include "./modules/args_parser/constants.h"
#include "./modules/shared_memory/shared_memory.c"
#include "./modules/tcp_performConnection/performConnection.h"
#include "./modules/tcp_performConnection/tcp_connection.h"

#include <arpa/inet.h> // For ntohs()
#include <errno.h>
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

int shmid; // Second SHM segment ID for game state
char *shm; // Pointer to second SHM segment

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

  // Create first SHM segment??

  // Create second SHM segment
  createBoardMemory();

  // Create buffer to store piece data
  char piece_data[INITIAL_SIZE] = "";

  // Create an inter-process communication pipe
  if (create_pipe() != 0) {
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
 * @brief Initialize the game configuration and application configuration
 * @param argc Argument count
 * @param argv Argument vector
 * @param game_config Pointer to GameConfig structure
 * @param app_config Pointer to Config structure
 * @return 0 on success, -1 on failure
 */
static int initialize_game(int argc, char *argv[], GameConfig *game_config,
                           Config *app_config) {
  if (!parse_args(argc, argv, game_config)) {
    print_usage(argv[0]);
    return -1;
  }

  printf("GAME-ID: %s\n", game_config->game_id);
  printf("Spielerzahl: %d\n", game_config->player_number);
  printf("Konfigurationsdatei: %s\n", game_config->config_file);

  if (!parse_config_file(game_config->config_file, app_config)) {
    fprintf(stderr, "Fehler: Ungültige Konfigurationsdatei. %s\n",
            game_config->config_file);
    return -1;
  }

  printf("Hostname: %s\n", app_config->hostname);
  printf("Portnummer: %u\n", ntohs(app_config->portNumber));
  printf("GameKindName: %s\n", app_config->gameKindName);

  return 0;
}

static void createBoardMemory() {
  // Creates second SHM segment
  shmid = shmget(IPC_PRIVATE, sizeof(char) * INITIAL_SIZE, IPC_CREAT | 0666);
  if (shmid < 0) {
    fprintf(stderr, "Shared memory creation for board failed.");
    exit(EXIT_FAILURE);
  } else {
    fprintf(stdout, "Shared memory creation for board was sucessful.\n");
  }

  shm = (char *) shmat(shmid, NULL, 0);
  if (shm == (char *) -1) {
    fprintf(stderr, "shmat failed.");
    exit(EXIT_FAILURE);
  } else {
    fprintf(stdout, "shmat was sucessful.\n");
  }
}

/**
 * @brief Create a pipe for inter-process communication
 * @return 0 on success, -1 on failure
 */
static int create_pipe() {
  if (pipe(pipe_fd) == -1) {
    fprintf(stderr, "Failed to create pipe: %s\n", strerror(errno));
    return -1;
  }
  return 0;
}

/**
 * @brief Fork processes and manage their execution
 * @param game_config The configuration for the game
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
static int fork_processes(GameConfig game_config, char *piece_data) {
  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork failed: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  if (pid == 0) {
    run_connector(game_config, piece_data);
  } else {
    run_thinker(pid);
  }

  return EXIT_SUCCESS;
}

/**
 * @brief Run the connector process logic
 * @param game_config The configuration for the game
 */
static void run_connector(GameConfig game_config, char *piece_data) {
  // Close the read end of the pipe in the connector process
  if (close(pipe_fd[0]) == -1) {
    fprintf(stderr, "Connector: Error closing read end of the pipe: %s\n",
            strerror(errno));
  }

  if (createConnection(game_config.game_id, piece_data, shm) != 0) {
    fprintf(stderr, "Connector: Failed to establish connection.\n");
    // Close the write end of the pipe only if createConnection fails
    if (close(pipe_fd[1]) == -1) {
      fprintf(stderr, "Connector: Error closing write end of the pipe: %s\n",
              strerror(errno));
    }
    exit(EXIT_FAILURE);
  }

  // Close the write end of the pipe immediately after successful connection
  if (close(pipe_fd[1]) == -1) {
    fprintf(stderr, "Connector: Error closing write end of the pipe: %s\n",
            strerror(errno));
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

/**
 * @brief Run the thinker process logic
 * @param pid The process ID of the connector process
 */
static void run_thinker(pid_t pid) {
  // Close the write end of the pipe in the thinker process
  if (close(pipe_fd[1]) == -1) {
    fprintf(stderr, "Thinker: Error closing write end of the pipe: %s\n",
            strerror(errno));
  }

  int status;
  if (waitpid(pid, &status, 0) == -1) {
    fprintf(stderr, "Thinker: Error waiting for child process: %s\n",
            strerror(errno));
    if (close(pipe_fd[0]) == -1) {
      fprintf(stderr, "Thinker: Error closing read end of the pipe: %s\n",
              strerror(errno));
    }
    exit(EXIT_FAILURE); // Exit thinker if waitpid fails
  }

  if (WIFEXITED(status)) {
    printf("Connector exited with status %d.\n", WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    printf("Connector terminated by signal %d.\n", WTERMSIG(status));
  }

  // Close the read end of the pipe in the thinker process
  if (close(pipe_fd[0]) == -1) {
    fprintf(stderr, "Thinker: Error closing read end of the pipe: %s\n",
            strerror(errno));
  }

  fprintf(stdout, "Data in second SHM segment:\n%s\n", shm);

  // Disconnects SHM
  if (shmdt(shm) == -1) {
    fprintf(stderr, "shmdt failed.");
    exit(EXIT_FAILURE);
  } else {
    fprintf(stdout, "shmdt was sucessful.\n");
  }

  // Frees SHM segment
  if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    fprintf(stderr, "shmctl failed.");
    exit(EXIT_FAILURE);
  } else {
    fprintf(stdout, "shmctl was sucessful.\n");
  }
}
/**********************************************************
 *                END OF HELPER FUNCTIONS                 *
 *********************************************************/