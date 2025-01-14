#include "performConnection.h"

#include "gameplay.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

/**
 * @brief Sends a message over the socket.
 *
 * @param sockfd The socket file descriptor.
 * @param message The message to send.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int sendMessage(int sockfd, const char *message) {
  if (send(sockfd, message, strlen(message), 0) == -1) {
    fprintf(stderr, "Error sending message: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  fprintf(stdout, "C: %s", message);
  return EXIT_SUCCESS;
}

/**
 * @brief Receives a message over the socket.
 *
 * @param sockfd The socket file descriptor.
 * @param buffer The buffer to store the received message.
 * @param buffer_size The size of the buffer.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int receiveMessage(int sockfd, char *buffer, size_t buffer_size) {
  if (buffer_size
      <= 1) { // Check buffer_size to prevent overflow or invalid memory access
    fprintf(stderr, "Invalid buffer size.\n");
    return EXIT_FAILURE;
  }

  size_t total_received = 0; // Total length of the received data
  while (total_received < buffer_size - 1) {
    char temp_buffer[2] = {0}; // Temporary buffer for a single character
    ssize_t bytes_received =
      recv(sockfd, temp_buffer, 1, 0); // Receive one character

    if (bytes_received < 0) {
      fprintf(stderr, "Error receiving message: %s\n", strerror(errno));
      return EXIT_FAILURE;
    } else if (bytes_received == 0) {
      fprintf(stderr, "Connection closed by server.\n");
      return EXIT_FAILURE;
    } else if (bytes_received > 1) {
      fprintf(stderr, "Unexpected bytes received: %zd\n", bytes_received);
      return EXIT_FAILURE;
    }

    // Append the received character to the buffer
    buffer[total_received] = temp_buffer[0];
    total_received++;

    // Stop if a newline character is found
    if (temp_buffer[0] == '\n') {
      break;
    }
  }

  // Add a null terminator to make the buffer a valid string
  buffer[total_received] = '\0';

  fprintf(stdout, "S: %s", buffer);
  return EXIT_SUCCESS;
}

/**
 * @brief Executes the connection procedure according to the communication
 * protocol.
 *
 * @param sockfd The socket file descriptor for the TCP connection.
 * @param GAME_ID The game ID to use for the connection.
 * @param piece_data Buffer for storing game state.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int performConnection(int sockfd, char *GAME_ID, char *piece_data) {
  char buffer[BUFFER_SIZE];

  // 1. Receive greeting from server
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Check the prefix "+ MNM Gameserver"
  if (strncmp(buffer, "+ MNM Gameserver", 16) != 0) {
    fprintf(stderr, "Unexpected server response: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // 2. Receive another message from server
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 3. Send client version
  const char *client_version = "VERSION 3.42\n";
  if (sendMessage(sockfd, client_version) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 4. Receive confirmation and game ID request
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ Client version accepted", 25) != 0) {
    fprintf(stderr, "Client version not accepted: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // 5. Send game ID // @noluyorAbi I THINK THIS WORKS NOW
  char game_id_message[BUFFER_SIZE]; // Use a stack-allocated buffer
  snprintf(game_id_message, BUFFER_SIZE, "ID %s\n", GAME_ID);
  if (sendMessage(sockfd, game_id_message) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 6. Receive game type
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ PLAYING NMMorris", 18) != 0) {
    fprintf(stderr, "Unexpected game type: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // 7. Receive game name (if required)
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 8. Send PLAYER command (no additional values)
  const char *player_command = "PLAYER\n";
  if (sendMessage(sockfd, player_command) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 9. Receive player assignment
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ YOU", 5) != 0) {
    fprintf(stderr, "Unexpected player assignment: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // 10. Receive total number of players
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ TOTAL ", 7) != 0) {
    fprintf(stderr, "Unexpected total player count: %s\n", buffer);
    return EXIT_FAILURE;
  }

  int total_players;
  if (sscanf(buffer + 7, "%d", &total_players) != 1) {
    fprintf(stderr, "Error parsing total player count: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // 11. Receive details of other players
  while (1) {
    if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    if (strncmp(buffer, "+ ENDPLAYERS", 12) == 0) {
      break;
    }
    int other_player_number;
    char other_player_name[50];
    int other_player_readiness;
    if (sscanf(buffer, "+ %d %49s %d", &other_player_number, other_player_name,
               &other_player_readiness)
        == 3) {
      printf("(S: Spieler %d (%s) ist %s.)\n", other_player_number,
             other_player_name,
             other_player_readiness == 1 ? "bereit" : "noch nicht bereit");
    } else {
      fprintf(stderr, "Unknown player info: %s\n", buffer);
    }
  }

  // Complete connection protocol
  while (1) {
    if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    if (strncmp(buffer, "+ WAIT", 6) == 0) {
      if (handleWait(sockfd, buffer) != 0) {
        return EXIT_FAILURE;
      }
    } else if (strncmp(buffer, "+ MOVE", 6) == 0) {
      if (handleMove(sockfd, buffer, piece_data) != 0) {
        return EXIT_FAILURE;
      }
    } else if (strncmp(buffer, "+ GAMEOVER", 10) == 0) {
      if (handleGameover(sockfd, buffer, piece_data) != 0) {
        return EXIT_FAILURE;
      }
      break; // Exit loop on GAMEOVER
    } else {
      fprintf(stderr, "Unrecognized message: %s\n", buffer);
      return EXIT_FAILURE;
    }
  }

  fprintf(stdout, "Prolog phase completed successfully.\n");
  return EXIT_SUCCESS;
}