#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <stddef.h>

/**
 * @brief Establishes a connection to the server and performs the protocol.
 *
 * @param GAME_ID The game ID to use for the connection.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int createConnection(char *GAME_ID);

#endif // TCPCONNECTION_H