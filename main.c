// ============================= INCLUDES ==============================
#include "./modules/args_parser/args_parser.h"
#include "./modules/args_parser/config.h"
#include "./modules/args_parser/constants.h"
#include "./modules/shared_memory/shared_memory.c"
#include "./modules/tcp_performConnection/performConnection.h"
#include "./modules/tcp_performConnection/tcp_connection.h"

#include <arpa/inet.h> // For ntohs()
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// ========================= GLOBAL VARIABLES ==========================
int pipe_fd[2]; // Pipe file descriptors: [0] read, [1] write

// ========================= FUNCTION PROTOTYPES =======================
static int initialize_game(int argc, char *argv[], GameConfig *game_config,
                           Config *app_config);
static int create_pipe();
static int fork_processes(GameConfig game_config);
static void run_connector(GameConfig game_config);
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

  // Create an inter-process communication pipe
  if (create_pipe() != 0) {
    return EXIT_FAILURE;
  }

  // Fork processes for connector and thinker logic
  return fork_processes(game_config);
}
/**********************************************************
 *                   END OF MAIN FUNCTION                 *
 *********************************************************/

// ========================= ---- ======================= //

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

/**
 * @brief Create a pipe for inter-process communication
 * @return 0 on success, -1 on failure
 */
static int create_pipe() {
  if (pipe(pipe_fd) == -1) {
    fprintf(stderr, "Failed to create pipe.\n");
    return -1;
  }
  return 0;
}

/**
 * @brief Fork processes and manage their execution
 * @param game_config The configuration for the game
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
static int fork_processes(GameConfig game_config) {
  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork failed.\n");
    return EXIT_FAILURE;
  }

  if (pid == 0) {
    run_connector(game_config);
  } else {
    run_thinker(pid);
  }

  return EXIT_SUCCESS;
}

/**
 * @brief Run the connector process logic
 * @param game_config The configuration for the game
 */
static void run_connector(GameConfig game_config) {
  if (close(pipe_fd[0]) == -1) {
    perror("Error closing read end of the pipe");
  }

  if (createConnection(game_config.game_id) != 0) {
    fprintf(stderr, "Connector: Failed to establish connection.\n");
    if (close(pipe_fd[1]) == -1) {
      perror("Error closing write end of the pipe");
    }
    exit(EXIT_FAILURE);
  }

  if (close(pipe_fd[1]) == -1) {
    perror("Error closing write end of the pipe");
  }
  exit(EXIT_SUCCESS);
}

/**
 * @brief Run the thinker process logic
 * @param pid The process ID of the connector process
 */
static void run_thinker(pid_t pid) {
  if (close(pipe_fd[1]) == -1) {
    perror("Error closing write end of the pipe");
  }

  int status;
  if (waitpid(pid, &status, 0) == -1) {
    fprintf(stderr, "Error waiting for child process.\n");
    if (close(pipe_fd[0]) == -1) {
      perror("Error closing read end of the pipe");
    }
    exit(EXIT_FAILURE);
  }

  if (WIFEXITED(status)) {
    printf("Connector exited with status %d.\n", WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    printf("Connector terminated by signal %d.\n", WTERMSIG(status));
  }

  if (close(pipe_fd[0]) == -1) {
    perror("Error closing read end of the pipe");
  }
}
/**********************************************************
 *                END OF HELPER FUNCTIONS                 *
 *********************************************************/