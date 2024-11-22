#ifndef PERFORMCONNECTION_H
#define PERFORMCONNECTION_H

/**
 * @brief Handles the Prolog phase of the communication protocol.
 *
 * @param sockfd The socket file descriptor for the TCP connection.
 * @return int 0 on success, -1 on error.
 */
int performConnection(int sockfd);

#endif // PERFORMCONNECTION_H