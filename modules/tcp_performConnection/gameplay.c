#include "gameplay.h"

#include "performConnection.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int handleWait(int sockfd, const char *waitLine) {
  if (sendMessage(sockfd, "OKWAIT\n") != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int handleMove(int sockfd, const char *moveLine, char *piece_data, char *shm) {
  char buffer[BUFFER_SIZE];

  // Receive "+ CAPTURE ..." immediately after + MOVE
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  if (strncmp(buffer, "+ CAPTURE", 9) != 0) {
    fprintf(stderr, "Unexpected response for CAPTURE: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // Receive a list of pieces until "+ ENDPIECELIST"
  while (1) {
    if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
    // Store piece data in second SHM segment
    if (strncmp(buffer, "+ PIECE", 7) == 0) {
      strcat(piece_data, buffer); // Adds PIECE data to piece_data buffer
      continue;
    }
    if (strncmp(buffer, "+ ENDPIECELIST", 14) == 0) {
      snprintf(shm, BUFFER_SIZE, "%s",
               piece_data); // Save collected data in SHM segment
      break;
    }
  }

  // Send "THINKING"
  if (sendMessage(sockfd, "THINKING\n") != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Receive "+ OKTHINK"
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  if (strncmp(buffer, "+ OKTHINK", 9) != 0) {
    fprintf(stderr, "Unexpected response for THINKING: %s\n", buffer);
    return EXIT_FAILURE;
  }

/* 
  // TODO: Send "PLAY" with think()-method, this was just a test
  DEBUG: WORKS
  if (sendMessage(sockfd, "PLAY B6\n") != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
 */


  // Receive "+ MOVEOK"
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  if (strncmp(buffer, "+ MOVEOK", 8) != 0) {
    fprintf(stderr, "Unexpected response for MOVEOK: %s\n", buffer);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int handleGameover(int sockfd, const char *gameoverLine, char *piece_data,
                   char *shm) {
  char buffer[BUFFER_SIZE];

  // Receive a list of pieces until "+ ENDPIECELIST"
  while (1) {
    if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
    // Store piece data in SHM segment
    if (strncmp(buffer, "+ PIECE", 7) == 0) {
      strcat(piece_data, buffer); // Adds PIECE data to piece_data buffer
      continue;
    }
    if (strncmp(buffer, "+ ENDPIECELIST", 14) == 0) {
      snprintf(shm, BUFFER_SIZE, "%s",
               piece_data); // Save collected data in SHM segment
      break;
    }
  }

  // Receive lines like "+ PLAYER0WON" or similar
  // We'll read 2 lines (example) that indicate winner/loser
  for (int i = 0; i < 2; i++) {
    if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
    fprintf(stdout, "%s", buffer);
  }

  // Receive "+ QUIT"
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  if (strncmp(buffer, "+ QUIT", 6) != 0) {
    fprintf(stderr, "Unexpected response for QUIT: %s\n", buffer);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}