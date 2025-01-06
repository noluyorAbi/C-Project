#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "../shared_memory/shared_memory.h"
#include "../tcp_performConnection/performConnection.h"

#define BUFFER_SIZE 1024

/**
 * @brief Runs the connector process, managing communication with the server and
 * thinker.
 * @param pipe_fd The write-end of the pipe to communicate with thinker.
 * @param sockfd The socket descriptor for communication with the server.
 */
void run_connector(int pipe_fd, int sockfd);

#endif