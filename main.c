// main.c

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

int pipe_fd[2]; // Pipe file descriptors: [0] read, [1] write

int main(int argc, char *argv[]) {
  GameConfig game_config;

  // Parse command-line arguments
  if (!parse_args(argc, argv, &game_config)) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }

  // Output the parsed command-line arguments
  printf("GAME-ID: %s\n", game_config.game_id);
  printf("Spielerzahl: %d\n", game_config.player_number);
  printf("Konfigurationsdatei: %s\n", game_config.config_file);

  // Parse the configuration file
  Config app_config;
  if (!parse_config_file(game_config.config_file, &app_config)) {
    fprintf(stderr, "Fehler: Ungültige Konfigurationsdatei. %s\n",
            game_config.config_file);
    return EXIT_FAILURE;
  }
  // Output the configured parameters
  printf("Hostname: %s\n", app_config.hostname);
  printf("Portnummer: %u\n",
         ntohs(app_config.portNumber)); // Converts back to host byte order
  printf("GameKindName: %s\n", app_config.gameKindName);

  // Create the pipe
  if (pipe(pipe_fd) == -1) {
    fprintf(stderr, "Failed to create pipe.");
    return EXIT_FAILURE;
  }

  // Fork to create Thinker and Connector processes
  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork failed for Thinker.");
    return EXIT_FAILURE;
  }

  if (pid == 0) {      // Connector process
    close(pipe_fd[0]); // Close the read end of the pipe

    if (createConnection(game_config.game_id) != 0) {
      fprintf(stderr, "Connector: Failed to establish connection.\n");
      close(pipe_fd[1]); // Close the write end of the pipe
      exit(EXIT_FAILURE);
    }

    close(pipe_fd[1]); // Close the write end of the pipe
    exit(EXIT_SUCCESS);
  } else {             // Thinker process
    close(pipe_fd[1]); // Close the write end of the pipe

    // Thinker process doesn't have a functionality yet.
    // Will be implemented later.

    // Wait for connector process to complete
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) { // Connector process exited with exit()
      printf("Connector exited with status %d.\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) { // Connector process exited with signal
      printf("Connector terminated by signal %d.\n", WTERMSIG(status));
    }

    close(pipe_fd[0]); // Close the read end of the pipe
    return EXIT_SUCCESS;
  }
}