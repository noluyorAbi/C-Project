#include "./modules/args_parser/args_parser.h"
#include "./modules/args_parser/config.h"
#include "./modules/tcp_performConnection/performConnection.h" // Include for performConnection
#include "./modules/thinker_and_connector/connector.h" // Include for run_connector
#include "./modules/thinker_and_connector/connector.h"
#include "./modules/thinker_and_connector/thinker.h"

#include <arpa/inet.h> // For ntohs()
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
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
  printf("Portnummer: %u\n", ntohs(app_config.portNumber));
  printf("GameKindName: %s\n", app_config.gameKindName);

  // Create the pipe
  if (pipe(pipe_fd) == -1) {
    perror("Failed to create pipe");
    return EXIT_FAILURE;
  }

  pid_t thinker_pid = fork();
  if (thinker_pid < 0) {
    perror("Fork failed for Thinker");
    return EXIT_FAILURE;
  }

  if (thinker_pid == 0) {
    // Thinker process
    close(pipe_fd[0]); // Close unused read end
    execl("./thinker", "./thinker", NULL);

    // If execl fails
    perror("execl failed for Thinker");
    return EXIT_FAILURE;
  }

  pid_t connector_pid = fork();
  if (connector_pid < 0) {
    perror("Fork failed for Connector");
    return EXIT_FAILURE;
  }

  if (connector_pid == 0) {
    // Connector process
    close(pipe_fd[1]); // Close unused write end
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
      perror("Failed to create socket");
      return EXIT_FAILURE;
    }

    // Establish server connection
    if (performConnection(sockfd) != EXIT_SUCCESS) {
      fprintf(stderr, "Failed to connect to server.\n");
      close(sockfd);
      return EXIT_FAILURE;
    }

    run_connector(pipe_fd[0], sockfd); // Call the run_connector function
    return EXIT_SUCCESS;
  }

  // Parent process
  close(pipe_fd[0]); // Close unused read end
  close(pipe_fd[1]); // Close unused write end

  // Wait for both child processes to complete
  int status;
  waitpid(thinker_pid, &status, 0);
  if (WIFEXITED(status)) {
    printf("Thinker exited with status %d.\n", WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    printf("Thinker terminated by signal %d.\n", WTERMSIG(status));
  }

  waitpid(connector_pid, &status, 0);
  if (WIFEXITED(status)) {
    printf("Connector exited with status %d.\n", WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    printf("Connector terminated by signal %d.\n", WTERMSIG(status));
  }

  return EXIT_SUCCESS;
}