#include "gameplay.h"

#include "performConnection.h"

#include <errno.h>
#include <signal.h> // For signal handling
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int handleWait(int sockfd, const char *waitLine) {
  if (sendMessage(sockfd, "OKWAIT\n") != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int checkForSignal(int sockfd) {
  fd_set read_fds;
  struct timeval timeout;
  char buffer[BUFFER_SIZE];

  while (1) {
    // Set the timeout for select (e.g., 5 seconds)
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    // Clear the file descriptor set and add the socket and pipe
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);     // Gameserver socket
    FD_SET(pipe_fd[0], &read_fds); // Pipe from Thinker (read end)

    // Wait for either the socket or pipe to be ready to read
    int ready_fds = select(FD_SETSIZE, &read_fds, NULL, NULL, &timeout);
    if (ready_fds == -1) {
      fprintf(stdout, "Select.\n");
      return EXIT_FAILURE;
    }

    if (ready_fds == 0) {
      // Timeout - nothing to read
      fprintf(stdout, "Timeout occurred. No data received.\n");
      continue;
    }

    // Check if the Gameserver socket has data
    if (FD_ISSET(sockfd, &read_fds)) {
      if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
      }
      if (strncmp(buffer, "- TIMEOUT", 9) == 0) {
        fprintf(stderr, "Received timeout from server: %s\n", buffer);
        return EXIT_FAILURE;
      } else if (strncmp(buffer, "- ERROR", 7) == 0) {
        fprintf(stderr, "Error from the server: %s\n", buffer);
        return EXIT_FAILURE;
      } else {
        // Handle other server messages as needed
        fprintf(stdout, "Received from server: %s\n", buffer);
      }
    }

    // Check if the Thinker pipe has data
    if (FD_ISSET(pipe_fd[0], &read_fds)) { // Read from pipe_fd[0]
      int n = read(pipe_fd[0], buffer,
                   sizeof(buffer)); // Read from the pipe's read end
      if (n == -1) {
        fprintf(stderr, "Read from pipe.\n");
        return EXIT_FAILURE;
      } else if (n == 0) {
        fprintf(stdout, "Thinker has closed the pipe.\n");
        return EXIT_FAILURE;
      } else {
        fprintf(stdout, "Connector: Read %d bytes from the pipe sucessfully.\n",
                n);
      }

      // Process the move sent by the Thinker
      buffer[n] = '\0';
      // fprintf(stdout, "Received move from Thinker: %s\n", buffer);

      // Send the move to the server following the protocol
      if (sendMessage(sockfd, buffer) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
      }

      // Exit the loop after successfully sending the move
      break;
    }
  }

  return EXIT_SUCCESS;
}

int handleMove(int sockfd, const char *moveLine, char *piece_data) {
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
      // Use strncat to prevent buffer overflow
      // Calculate remaining space in piece_data
      size_t current_length = strlen(piece_data);
      size_t remaining_space =
        BUFFER_SIZE - current_length - 1; // -1 for null terminator
      strncat(piece_data, buffer, remaining_space);
      continue;
    }
    if (strncmp(buffer, "+ ENDPIECELIST", 14) == 0) {
      // Save collected data in SHM segment
      // Use snprintf to ensure no overflow
      snprintf(shm, BUFFER_SIZE, "%s", piece_data);
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

  // Send SIGUSR1 signal to the Thinker process
  if (kill(getppid(), SIGUSR1) == -1) {
    fprintf(stderr, "Gameplay: Failed to send SIGUSR1 to Thinker: %s\n",
            strerror(errno));
    return EXIT_FAILURE;
  }

  // Check TIMEOUT and pipe to send MOVE
  checkForSignal(sockfd);

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

int handleGameover(int sockfd, const char *gameoverLine, char *piece_data) {
  char buffer[BUFFER_SIZE];

  // Receive a list of pieces until "+ ENDPIECELIST"
  while (1) {
    if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
    // Store piece data in SHM segment
    if (strncmp(buffer, "+ PIECE", 7) == 0) {
      // Use strncat to prevent buffer overflow
      // Calculate remaining space in piece_data
      size_t current_length = strlen(piece_data);
      size_t remaining_space =
        BUFFER_SIZE - current_length - 1; // -1 for null terminator
      strncat(piece_data, buffer, remaining_space);
      continue;
    }
    if (strncmp(buffer, "+ ENDPIECELIST", 14) == 0) {
      // Save collected data in SHM segment
      // Use snprintf to ensure no overflow
      snprintf(shm, BUFFER_SIZE, "%s", piece_data);
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