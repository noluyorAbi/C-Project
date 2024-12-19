#ifndef CONNECTOR_H
#define CONNECTOR_H

/**
 * @brief Runs the Connector logic, handling input from the pipe and the server.
 *
 * @param pipe_fd The file descriptor for the read end of the pipe.
 * @param sockfd The socket file descriptor for communication with the server.
 */
void run_connector(int pipe_fd, int sockfd);

#endif // CONNECTOR_H