#include "performConnection.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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
 * @brief Receives a message over the socket, handling fragmented messages.
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
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int performConnection(int sockfd) {
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

  // 2. Send client version
  const char *client_version = "VERSION 2.42\n";
  if (sendMessage(sockfd, client_version) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 3. Receive confirmation and game ID request
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ Client version accepted", 25) != 0) {
    fprintf(stderr, "Client version not accepted: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // 4. Send game ID
  const char *game_id = "ID my-game-id\n";
  if (sendMessage(sockfd, game_id) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 5. Receive game type
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ PLAYING NMMorris", 18) != 0) {
    fprintf(stderr, "Unexpected game type: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // 6. Receive game name (if required)
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 7. Send PLAYER command (no additional values)
  const char *player_command = "PLAYER\n";
  if (sendMessage(sockfd, player_command) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 8. Receive player assignment
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ YOU", 5) != 0) {
    fprintf(stderr, "Unexpected player assignment: %s\n", buffer);
    return EXIT_FAILURE;
  } else {
    fprintf(stdout, "Assigned player: %s", buffer + 5);
  }

  // 9. Receive total number of players
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ TOTAL", 7) != 0) {
    fprintf(stderr, "Unexpected total player count: %s\n", buffer);
    return EXIT_FAILURE;
  } else {
    int total_players;
    if (sscanf(buffer, "+ TOTAL %d", &total_players) == 1) {
      fprintf(stdout, "Total players: %d\n", total_players);
    } else {
      fprintf(stderr, "Error parsing total player count: %s\n", buffer);
      return EXIT_FAILURE;
    }
  }

  // 10. Receive details of other players
  while (1) {
    if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    if (strncmp(buffer, "+ ENDPLAYERS", 12) == 0) {
      break;
    }
    int player_number;
    char player_name[50];
    char readiness[20];
    if (sscanf(buffer, "+ PLAYER %d %49s %19s", &player_number, player_name,
               readiness)
        == 3) {
      fprintf(stdout, "Spieler %d (%s) ist %s.\n", player_number, player_name,
              strcmp(readiness, "READY") == 1 ? "bereit." : "nicht bereit.");
    } else {
      fprintf(stderr, "Unknown player info: %s\n", buffer);
    }
  }

  fprintf(stdout, "Prolog phase completed successfully.\n");
  return EXIT_SUCCESS;
}

/**
 * @brief Establishes a TCP connection and starts the protocol.
 *
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int createConnection() {
  int sock = -1;
  struct addrinfo hints = {0}, *result, *rp;

  hints.ai_family = AF_UNSPEC;     // Support both IPv4 and IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP connection

  if (getaddrinfo(HOSTNAME, PORT, &hints, &result) != 0) {
    fprintf(stderr, "Error resolving hostname.\n");
    return EXIT_FAILURE;
  }

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sock == -1) {
      continue; // Try the next address
    }

    if (connect(sock, rp->ai_addr, rp->ai_addrlen) == 0) {
      break; // Successfully connected
    }

    close(sock);
    sock = -1;
  }

  freeaddrinfo(result);

  if (sock == -1) {
    fprintf(stderr, "Error: Could not establish connection.\n");
    return EXIT_FAILURE;
  }

  if (performConnection(sock) != EXIT_SUCCESS) {
    close(sock);
    return EXIT_FAILURE;
  }

  close(sock);
  return EXIT_SUCCESS;
}