// connector.c

#include "performConnection.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h> // Use select instead of epoll
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

/**
 * Reads moves from the pipe and sends them to the server.
 */
void handle_pipe_input(int sockfd, int pipe_fd) {
  char move[BUFFER_SIZE];
  ssize_t bytes_read = read(pipe_fd, move, sizeof(move) - 1);

  if (bytes_read > 0) {
    move[bytes_read] = '\0'; // Null-terminate the move
    printf("Connector: Received move from Thinker: %s", move);

    // Send move to server
    if (sendMessage(sockfd, move) != EXIT_SUCCESS) {
      fprintf(stderr, "Connector: Failed to send move to server.\n");
      exit(EXIT_FAILURE);
    }
    printf("Connector: Move sent to server.\n");
  } else if (bytes_read == 0) {
    printf("Connector: Pipe closed by Thinker.\n");
    exit(EXIT_SUCCESS);
  } else {
    perror("Connector: Failed to read from pipe");
    exit(EXIT_FAILURE);
  }
}

/**
 * Runs the Connector logic.
 */
void run_connector(int pipe_fd, int sockfd) {
  fd_set read_fds; // Set of file descriptors to monitor

  printf("Connector: Waiting for input...\n");

  while (1) {
    FD_ZERO(&read_fds);         // Clear the set
    FD_SET(pipe_fd, &read_fds); // Add pipe_fd to the set
    FD_SET(sockfd, &read_fds);  // Add sockfd to the set

    int max_fd = (pipe_fd > sockfd) ? pipe_fd : sockfd;

    // Use select to monitor file descriptors
    int ret = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
    if (ret == -1) {
      perror("Connector: select failed");
      exit(EXIT_FAILURE);
    }

    // Check if pipe_fd is ready to read
    if (FD_ISSET(pipe_fd, &read_fds)) {
      handle_pipe_input(sockfd, pipe_fd);
    }

    // Check if sockfd is ready to read
    if (FD_ISSET(sockfd, &read_fds)) {
      char buffer[BUFFER_SIZE];
      if (receiveMessage(sockfd, buffer, sizeof(buffer)) != EXIT_SUCCESS) {
        fprintf(stderr, "Connector: Server connection error.\n");
        exit(EXIT_FAILURE);
      }
      printf("Connector: Server response: %s", buffer);
    }
  }

  close(sockfd);
  close(pipe_fd);
}