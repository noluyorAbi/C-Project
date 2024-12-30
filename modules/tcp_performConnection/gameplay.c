#include "gameplay.h"

#include "performConnection.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

// Phase: WAIT
int handleWait(int sockfd) {
  char buffer[BUFFER_SIZE];
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ WAIT", 6) != 0) {
    fprintf(stderr, "Unexpected response for WAIT: %s\n", buffer);
    return EXIT_FAILURE;
  }

  if (sendMessage(sockfd, "OKWAIT\n") != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// Phase: MOVE
int handleMove(int sockfd) {
  char buffer[BUFFER_SIZE];

  // Receive MOVE command
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  if (strncmp(buffer, "+ MOVE", 6) != 0) {
    fprintf(stderr, "Unexpected response for MOVE: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // Receive CAPTURE command
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  if (strncmp(buffer, "+ CAPTURE", 9) != 0) {
    fprintf(stderr, "Unexpected response for CAPTURE: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // Receive PIECELIST
  while (1) {
    if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    if (strncmp(buffer, "+ ENDPIECELIST", 14) == 0) {
      break;
    }
  }

  // Send THINKING
  if (sendMessage(sockfd, "THINKING\n") != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Receive OKTHINK command
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  if (strncmp(buffer, "+ OKTHINK", 9) != 0) {
    fprintf(stderr, "Unexpected response for THINKING: %s\n", buffer);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// Phase: GAMEOVER
int handleGameover(int sockfd) {
  char buffer[BUFFER_SIZE];

  // Receive GAMEOVER
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  if (strncmp(buffer, "+ GAMEOVER", 10) != 0) {
    fprintf(stderr, "Unexpected response for GAMEOVER: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // Receive PIECELIST
  while (1) {
    if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    if (strncmp(buffer, "+ ENDPIECELIST", 14) == 0) {
      break;
    }
  }

  // Receive PLAYERXWON messages
  for (int i = 0; i < 2; i++) {
    if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    fprintf(stdout, "%s", buffer); // Print winning status
  }

  // Receive QUIT
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  if (strncmp(buffer, "+ QUIT", 6) != 0) {
    fprintf(stderr, "Unexpected response for QUIT: %s\n", buffer);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}