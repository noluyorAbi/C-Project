#include "performConnection.h"

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
 * @return int 0 on success, -1 on error.
 */
int sendMessage(int sockfd, const char *message) {
  if (send(sockfd, message, strlen(message), 0) == -1) {
    perror("Error sending message");
    return EXIT_FAILURE;
  }
  printf("Sent: %s", message);
  return 0;
}

/**
 * @brief Receives a message over the socket.
 *
 * @param sockfd The socket file descriptor.
 * @param buffer The buffer to store the received message.
 * @param buffer_size The size of the buffer.
 * @return int 0 on success, -1 on error.
 */
int receiveMessage(int sockfd, char *buffer, size_t buffer_size) {
  int bytes_received = recv(sockfd, buffer, buffer_size - 1, 0);
  if (bytes_received <= 0) {
    perror("Error receiving message");
    return -1;
  }
  buffer[bytes_received] = '\0';
  printf("Received: %s", buffer);
  return EXIT_SUCCESS;
}

/**
 * @brief Handles the Prolog phase of the protocol.
 *
 * @param sockfd The socket file descriptor.
 * @return int 0 on success, -1 on error.
 */
int performConnection(int sockfd) {
  char buffer[BUFFER_SIZE];

  // 1. Receive greeting from server
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != 0) {
    return EXIT_FAILURE;
  }

  // Check for the "+ MNM Gameserver" prefix
  if (strncmp(buffer, "+ MNM Gameserver", 16) != 0) {
    fprintf(stderr, "Unexpected server response: %s\n", buffer);
    return -1;
  }

  // 2. Send client version
  const char *client_version = "VERSION 2.42\n";
  if (sendMessage(sockfd, client_version) != 0) {
    return -1;
  }

  // 3. Receive acknowledgment and Game-ID prompt
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != 0) {
    return -1;
  }

  if (strncmp(buffer, "+ Client version accepted", 25) != 0) {
    fprintf(stderr, "Client version not accepted: %s\n", buffer);
    return -1;
  }

  // 4. Send Game-ID
  const char *game_id = "ID my-game-id\n"; // Replace with the actual Game-ID
  if (sendMessage(sockfd, game_id) != 0) {
    return -1;
  }

  // 5. Receive game type
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != 0) {
    return -1;
  }

  if (strncmp(buffer, "+ PLAYING NMMorris", 18) != 0) {
    fprintf(stderr, "Unexpected game type: %s\n", buffer);
    return -1;
  }

  // 6. Receive game name
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != 0) {
    return -1;
  }

  // 7. Send PLAYER command
  const char *player_command = "PLAYER\n";
  if (sendMessage(sockfd, player_command) != 0) {
    return -1;
  }

  // 8. Receive player assignment
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != 0) {
    return -1;
  }

  if (strncmp(buffer, "+ YOU", 5) != 0) {
    fprintf(stderr, "Unexpected player assignment: %s\n", buffer);
    return -1;
  }

  // 9. Receive total players
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != 0) {
    return -1;
  }

  if (strncmp(buffer, "+ TOTAL", 7) != 0) {
    fprintf(stderr, "Unexpected total players: %s\n", buffer);
    return -1;
  }

  // 10. Receive details of other players
  while (1) {
    if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != 0) {
      return -1;
    }
    if (strncmp(buffer, "+ ENDPLAYERS", 12) == 0) {
      break; // End of player list
    }
    // Process player information (e.g., number, name, readiness)
    printf("Player info: %s", buffer);
  }

  printf("Prolog phase completed successfully.\n");
  return 0; // Success
}