// main.c

#include "./modules/args_parser/args_parser.h"
#include "./modules/args_parser/config.h"
#include "./modules/args_parser/constants.h" // Inclusion of constants, if needed

#include <arpa/inet.h> // For ntohs()
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

  pid_t pid = fork();

  if (pid < 0) {
    // Fork failed
    perror("Fork failed");
    return EXIT_FAILURE;
  }

  if (pid == 0) {
    // Child process: Connector
    printf("Connector process (PID: %d) starting...\n", getpid());

    // Execute the connector
    char thinker_pid_str[16];
    snprintf(thinker_pid_str, sizeof(thinker_pid_str), "%d",
             getppid()); // Thinker PID is the parent's PID
    execl("./thinker_and_connector/connector",
          "./thinker_and_connector/connector", thinker_pid_str, (char *) NULL);

    // If execl fails
    perror("execl failed for connector");
    return EXIT_FAILURE;
  } else {
    // Parent process: Thinker
    printf("Thinker process (PID: %d) waiting for connector...\n", getpid());

    // Call thinker logic here
    // You can implement thinker logic in a function or directly here
    printf("Thinker is running...\n");
    // Simulate thinker work (e.g., waiting for a signal)
    while (1) {
      pause(); // Wait for signals (e.g., from connector)
    }

    // Wait for the child process to finish
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
      printf("Connector exited with status %d.\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
      printf("Connector terminated by signal %d.\n", WTERMSIG(status));
    }
  }

  return EXIT_SUCCESS;
}
