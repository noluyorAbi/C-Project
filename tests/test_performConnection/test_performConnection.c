// tests/test_performConnection.c

#include "../../modules/tcp_performConnection/performConnection.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

/**
 * @brief Mock-Server-Funktion, die erwartete Nachrichten sendet und empfängt.
 *
 * @param arg Ein Zeiger auf den Server-Socket.
 * @return void* Nichts
 */
void *mock_server(void *arg) {
  int server_sock = *(int *) arg;
  char buffer[BUFFER_SIZE];

  // 1. Sende Begrüßung
  const char *greeting = "+ MNM Gameserver\n";
  if (send(server_sock, greeting, strlen(greeting), 0) == -1) {
    perror("Mock Server: Fehler beim Senden der Begrüßung");
    close(server_sock);
    return NULL;
  }
  fprintf(stdout, "Mock Server Gesendet: %s", greeting);

  // 2. Empfang der Client-Version
  ssize_t bytes = recv(server_sock, buffer, BUFFER_SIZE - 1, 0);
  if (bytes <= 0) {
    perror("Mock Server: Fehler beim Empfangen der Client-Version");
    close(server_sock);
    return NULL;
  }
  buffer[bytes] = '\0';
  fprintf(stdout, "Mock Server Empfangen: %s", buffer);

  if (strncmp(buffer, "VERSION", 7) == 0) {
    const char *ack = "+ Client version accepted\n";
    if (send(server_sock, ack, strlen(ack), 0) == -1) {
      perror("Mock Server: Fehler beim Senden der Bestätigung");
      close(server_sock);
      return NULL;
    }
    fprintf(stdout, "Mock Server Gesendet: %s", ack);
  } else {
    const char *nack = "- Unsupported version\n";
    if (send(server_sock, nack, strlen(nack), 0) == -1) {
      perror("Mock Server: Fehler beim Senden der Ablehnung");
    }
    fprintf(stdout, "Mock Server Gesendet: %s", nack);
    close(server_sock);
    return NULL;
  }

  // 3. Empfang der Game-ID
  bytes = recv(server_sock, buffer, BUFFER_SIZE - 1, 0);
  if (bytes <= 0) {
    perror("Mock Server: Fehler beim Empfangen der Game-ID");
    close(server_sock);
    return NULL;
  }
  buffer[bytes] = '\0';
  fprintf(stdout, "Mock Server Empfangen: %s", buffer);

  // Bestätigen des Game-ID Empfangs
  const char *game_type = "+ PLAYING NMMorris\n";
  if (send(server_sock, game_type, strlen(game_type), 0) == -1) {
    perror("Mock Server: Fehler beim Senden des Spieltyps");
    close(server_sock);
    return NULL;
  }
  fprintf(stdout, "Mock Server Gesendet: %s", game_type);

  // 4. Empfang des PLAYER-Befehls
  bytes = recv(server_sock, buffer, BUFFER_SIZE - 1, 0);
  if (bytes <= 0) {
    perror("Mock Server: Fehler beim Empfangen des PLAYER-Befehls");
    close(server_sock);
    return NULL;
  }
  buffer[bytes] = '\0';
  fprintf(stdout, "Mock Server Empfangen: %s", buffer);

  if (strncmp(buffer, "PLAYER", 6) == 0) {
    const char *player_assignment = "+ YOU 1 Uli READY\n";
    if (send(server_sock, player_assignment, strlen(player_assignment), 0)
        == -1) {
      perror("Mock Server: Fehler beim Senden der Spielerzuweisung");
      close(server_sock);
      return NULL;
    }
    fprintf(stdout, "Mock Server Gesendet: %s", player_assignment);

    const char *total_players = "+ TOTAL 2\n";
    if (send(server_sock, total_players, strlen(total_players), 0) == -1) {
      perror("Mock Server: Fehler beim Senden der Gesamtanzahl der Spieler");
      close(server_sock);
      return NULL;
    }
    fprintf(stdout, "Mock Server Gesendet: %s", total_players);

    const char *player_info1 = "+ PLAYER 2 Bob NOT_READY\n";
    if (send(server_sock, player_info1, strlen(player_info1), 0) == -1) {
      perror("Mock Server: Fehler beim Senden der Spielerinformationen");
      close(server_sock);
      return NULL;
    }
    fprintf(stdout, "Mock Server Gesendet: %s", player_info1);

    const char *end_players = "+ ENDPLAYERS\n";
    if (send(server_sock, end_players, strlen(end_players), 0) == -1) {
      perror("Mock Server: Fehler beim Senden des Endes der Spielerliste");
      close(server_sock);
      return NULL;
    }
    fprintf(stdout, "Mock Server Gesendet: %s", end_players);
  }

  close(server_sock);
  return NULL;
}

int main() {
  int sockfd_pair[2];
  pthread_t server_thread;

  // Erstelle ein Socket-Paar
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd_pair) == -1) {
    perror("socketpair");
    exit(EXIT_FAILURE);
  }

  // Starte den Mock-Server in einem separaten Thread
  if (pthread_create(&server_thread, NULL, mock_server, &sockfd_pair[1]) != 0) {
    perror("pthread_create");
    close(sockfd_pair[0]);
    close(sockfd_pair[1]);
    exit(EXIT_FAILURE);
  }

  // Führe performConnection mit dem Client-Socket aus
  if (performConnection(sockfd_pair[0]) != EXIT_SUCCESS) {
    fprintf(stderr, "performConnection fehlgeschlagen.\n");
    close(sockfd_pair[0]);
    close(sockfd_pair[1]);
    pthread_join(server_thread, NULL);
    exit(EXIT_FAILURE);
  }

  // Schließe die Sockets und warte auf den Server-Thread
  close(sockfd_pair[0]);
  close(sockfd_pair[1]);
  pthread_join(server_thread, NULL);

  fprintf(stdout, "Test erfolgreich abgeschlossen.\n");
  return EXIT_SUCCESS;
}
