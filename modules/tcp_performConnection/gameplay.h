#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <stddef.h>

/**
 * @brief Handles the WAIT phase of the game.
 *
 * @param sockfd The socket file descriptor.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int handleWait(int sockfd);

/**
 * @brief Handles the MOVE phase of the game.
 *
 * @param sockfd The socket file descriptor.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int handleMove(int sockfd);

/**
 * @brief Handles the GAMEOVER phase of the game.
 *
 * @param sockfd The socket file descriptor.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int handleGameover(int sockfd);

#endif