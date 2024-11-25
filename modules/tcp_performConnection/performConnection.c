#include "performConnection.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

/**
 * @brief Sendet eine Nachricht über den Socket.
 *
 * @param sockfd Der Socket-Dateideskriptor.
 * @param message Die zu sendende Nachricht.
 * @return int EXIT_SUCCESS bei Erfolg, EXIT_FAILURE bei Fehler.
 */
int sendMessage(int sockfd, const char *message) {
  if (send(sockfd, message, strlen(message), 0) == -1) {
    fprintf(stderr, "Fehler beim Senden der Nachricht: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  fprintf(stdout, "Gesendet: %s", message);
  return EXIT_SUCCESS;
}

/**
 * @brief Empfängt eine Nachricht über den Socket.
 *
 * @param sockfd Der Socket-Dateideskriptor.
 * @param buffer Der Puffer zum Speichern der empfangenen Nachricht.
 * @param buffer_size Die Größe des Puffers.
 * @return int EXIT_SUCCESS bei Erfolg, EXIT_FAILURE bei Fehler.
 */
int receiveMessage(int sockfd, char *buffer, size_t buffer_size) {
  ssize_t bytes_received = recv(sockfd, buffer, buffer_size - 1, 0);
  if (bytes_received < 0) {
    fprintf(stderr, "Fehler beim Empfangen der Nachricht: %s\n",
            strerror(errno));
    return EXIT_FAILURE;
  } else if (bytes_received == 0) {
    fprintf(stderr, "Verbindung vom Server geschlossen\n");
    return EXIT_FAILURE;
  }
  buffer[bytes_received] = '\0';
  printf("Received: %s", buffer);
  return EXIT_SUCCESS;
}

/**
 * @brief Führt die Verbindungsprozedur gemäß dem Kommunikationsprotokoll durch.
 *
 * @param sockfd Der Socket-Dateideskriptor für die TCP-Verbindung.
 * @return int EXIT_SUCCESS bei Erfolg, EXIT_FAILURE bei Fehler.
 */
int performConnection(int sockfd) {
  char buffer[BUFFER_SIZE];

  // 1. Begrüßung vom Server empfangen
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // Überprüfung des Prefix "+ MNM Gameserver"
  if (strncmp(buffer, "+ MNM Gameserver", 16) != 0) {
    fprintf(stderr, "Unerwartete Serverantwort: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // 2. Client-Version senden
  const char *client_version = "VERSION 2.42\n";
  if (sendMessage(sockfd, client_version) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 3. Bestätigung und Game-ID-Aufforderung empfangen
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ Client version accepted", 25) != 0) {
    fprintf(stderr, "Client-Version nicht akzeptiert: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // 4. Game-ID senden
  const char *game_id = "ID my-game-id\n";
  if (sendMessage(sockfd, game_id) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 5. Spieltyp empfangen
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ PLAYING NMMorris", 18) != 0) {
    fprintf(stderr, "Unerwarteter Spieltyp: %s\n", buffer);
    return EXIT_FAILURE;
  }

  // 6. Spielname empfangen (falls erforderlich)
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 7. PLAYER-Befehl senden (ohne zusätzliche Werte)
  const char *player_command = "PLAYER\n";
  if (sendMessage(sockfd, player_command) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  // 8. Spielerzuweisung empfangen
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ YOU", 5) != 0) {
    fprintf(stderr, "Unerwartete Spielerzuweisung: %s\n", buffer);
    return EXIT_FAILURE;
  } else {
    fprintf(stdout, "Zugewiesener Spieler: %s", buffer + 5);
  }

  // 9. Gesamtanzahl der Spieler empfangen
  if (receiveMessage(sockfd, buffer, BUFFER_SIZE) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  if (strncmp(buffer, "+ TOTAL", 7) != 0) {
    fprintf(stderr, "Unerwartete Gesamtanzahl der Spieler: %s\n", buffer);
    return EXIT_FAILURE;
  } else {
    int total_players;
    if (sscanf(buffer, "+ TOTAL %d", &total_players) == 1) {
      fprintf(stdout, "Gesamtanzahl der Spieler: %d\n", total_players);
    } else {
      fprintf(stderr, "Fehler beim Parsen der Gesamtanzahl der Spieler: %s\n",
              buffer);
      return EXIT_FAILURE;
    }
  }

  // 10. Details der anderen Spieler empfangen
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
              strcmp(readiness, "READY") == 0 ? "bereit" : "nicht bereit");
    } else {
      fprintf(stderr, "Unbekannte Spielerinfo: %s\n", buffer);
    }
  }

  fprintf(stdout, "Prolog-Phase erfolgreich abgeschlossen.\n");
  return EXIT_SUCCESS;
}
