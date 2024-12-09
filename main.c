#include "./modules/args_parser/args_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define GAMEKINDNAME "NMMorris"

int main(int argc, char *argv[]) {
  GameConfig config;

  // Parse the command-line arguments
  if (!parse_args(argc, argv, &config)) {
    print_usage(argv[0]);
    return 1;
  }

  printf("GAME-ID: %s\n", config.game_id);
  printf("Spielerzahl: %d\n", config.player_number);

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

  return 0;
}