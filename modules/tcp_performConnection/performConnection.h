// performConnection.h

#ifndef PERFORMCONNECTION_H
#define PERFORMCONNECTION_H

#include <stddef.h>

/**
 * @brief Sendet eine Nachricht über den Socket.
 *
 * @param sockfd Der Socket-Dateideskriptor.
 * @param message Die zu sendende Nachricht.
 * @return int EXIT_SUCCESS bei Erfolg, EXIT_FAILURE bei Fehler.
 */
int sendMessage(int sockfd, const char *message);

/**
 * @brief Empfängt eine Nachricht über den Socket.
 *
 * @param sockfd Der Socket-Dateideskriptor.
 * @param buffer Der Puffer zum Speichern der empfangenen Nachricht.
 * @param buffer_size Die Größe des Puffers.
 * @return int EXIT_SUCCESS bei Erfolg, EXIT_FAILURE bei Fehler.
 */
int receiveMessage(int sockfd, char *buffer, size_t buffer_size);

/**
 * @brief Führt die Verbindungsprozedur gemäß dem Kommunikationsprotokoll durch.
 *
 * @param sockfd Der Socket-Dateideskriptor für die TCP-Verbindung.
 * @return int EXIT_SUCCESS bei Erfolg, EXIT_FAILURE bei Fehler.
 */
int performConnection(int sockfd);

#endif // PERFORMCONNECTION_H
