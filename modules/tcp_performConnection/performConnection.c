#include "performConnection.h"

#include "../args_parser/args_parser.h"
#include "../shared_memory/shared_memory.h"
#include "gameplay.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 32768

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
  // fprintf(stdout, "C: %s", message);
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

  // fprintf(stdout, "S: %s", buffer); SERVER RESPONSES FOR DEBUGGING
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
  fprintf(stdout, "\033[0;33m\n[DEBUG] PLAYER VALUE IN -p IST: %d\n\n\033[0m",
          EXTERN_PLAYER_NUMBER);
  // 1. Receive greeting from server
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Check the prefix "+ MNM Gameserver"
  if (strncmp(buffer, "+ MNM Gameserver", 16) != 0) {
    fprintf(stderr, "Unexpected server response: %s\n", buffer);
    return EXIT_FAILURE;
  }
  fprintf(stdout, "Verbindung zu Server hergestellt.\n");

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
  fprintf(stdout, "Client-Version wurde akzeptiert.\n");

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
  fprintf(stdout, "Das ausgewählte Spiel ist Neunermühle.\n");

  // 7. Receive game name (if required)
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  fprintf(stdout, "Der Spielname lautet: %s", buffer + 2);
  strncpy(shm_info->gameName, buffer + 2, sizeof(shm_info->gameName) - 1);
  shm_info->gameName[sizeof(shm_info->gameName) - 1] =
    '\0'; // Null-Terminierung sicherstellen

  // Entferne ein eventuell vorhandenes Newline-Zeichen
  char *newline = strchr(shm_info->gameName, '\n');
  if (newline) {
    *newline = '\0'; // Ersetze \n durch \0
  }

  // 8. Send PLAYER command (no additional values)
  // `Geben Sie beim PLAYER-Kommando keine Werte mit und lassen Sie sich vom
  // Gameserver einen freien Spieler zuweisen.`
  char *player_command;
  if (EXTERN_PLAYER_NUMBER == 0) {
    player_command = "PLAYER \n";
  } else if (EXTERN_PLAYER_NUMBER == 1) {
    player_command = "PLAYER 0\n";
  } else if (EXTERN_PLAYER_NUMBER == 2) {
    player_command = "PLAYER 1\n";
  } else {
    fprintf(stderr, "EXTERN_PLAYER_NUMBER is not valid.");
    return EXIT_FAILURE;
  }

  // const char *player_command = "PLAYER \n";
  if (sendMessage(sockfd, player_command) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 9. Receive player assignment
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // TODO: Ask Rufus about this
  if (strncmp(buffer, "+ YOU", 5) != 0) {
    fprintf(stderr,
            "Der Player den sie ausgewählt haben in -p <1|2> ist nicht "
            "frei. Wählen sie einen anderen. %s\n",
            buffer);
    return EXIT_FAILURE;
  }

  shm_info->playerNumber = atoi(&buffer[6]);

  if (shm_info->playerNumber == 0) {
    fprintf(stdout, "Du bist Spieler 0.\n");
  } else {
    fprintf(stdout, "Du bist Spieler 1.\n");
  }

  // Save our player info to SHM
  int our_player_number;
  char our_player_name[50];
  if (sscanf(buffer, "+ YOU %d %49s", &our_player_number, our_player_name)
      == 2) {
    shm_info->players[our_player_number].playerNumber = our_player_number;
    snprintf(shm_info->players[our_player_number].playerName,
             sizeof(shm_info->players[our_player_number].playerName), "%s",
             our_player_name);
    shm_info->players[our_player_number].isRegistered = true;
  }

  // 10. Receive total number of players
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ TOTAL ", 7) != 0) {
    fprintf(stderr, "Unexpected total player count: %s\n", buffer);
    return EXIT_FAILURE;
  }
  const int NUM_OF_PLAYERS = atoi(&buffer[8]);
  fprintf(stdout, "Es gibt insgesamt %d Spieler.\n", NUM_OF_PLAYERS);

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
      printf("Spieler %d (%s) ist %s.\n", other_player_number,
             other_player_name,
             other_player_readiness == 1 ? "bereit" : "noch nicht bereit");

      // Save other player info to SHM
      shm_info->players[other_player_number].playerNumber = other_player_number;
      snprintf(shm_info->players[other_player_number].playerName,
               sizeof(shm_info->players[other_player_number].playerName), "%s",
               other_player_name);
      shm_info->players[other_player_number].isRegistered =
        other_player_readiness;
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
      fprintf(stdout, "Warte einen Moment.\n");
      if (handleWait(sockfd, buffer) != 0) {
        return EXIT_FAILURE;
      }
    } else if (strncmp(buffer, "+ MOVE", 6) == 0) {
      fprintf(stdout, "Bewege einen Stein.\n");
      if (handleMove(sockfd, buffer, piece_data) != 0) {
        return EXIT_FAILURE;
      }
    } else if (strncmp(buffer, "+ GAMEOVER", 10) == 0) {
      fprintf(stdout, "Das Spiel ist beendet.\n");
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