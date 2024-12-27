#ifndef PERFORMCONNECTION_H
#define PERFORMCONNECTION_H

#include <stddef.h>

/**
 * @brief Sends a message over the socket.
 *
 * @param sockfd The socket file descriptor.
 * @param message The message to send.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int sendMessage(int sockfd, const char *message);

/**
 * @brief Receives a message over the socket.
 *
 * @param sockfd The socket file descriptor.
 * @param buffer The buffer to store the received message.
 * @param buffer_size The size of the buffer.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int receiveMessage(int sockfd, char *buffer, size_t buffer_size);

/**
 * @brief Executes the connection procedure according to the communication
 * protocol.
 *
 * @param sockfd The file descriptor for the TCP connection.
 * @param GAME_ID The game ID to use for the connection.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int performConnection(int sockfd, char *GAME_ID);

#endif