#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <stddef.h>

// TODO MAKE PARAMS

/**
 * @brief Handles the WAIT phase of the game.
 *
 * @param sockfd The socket file descriptor.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int handleWait(int sockfd, const char *waitLine);

/**
 * @brief Handles the MOVE phase of the game.
 *
 * @param sockfd The socket file descriptor.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int handleMove(int sockfd, const char *moveLine);

/**
 * @brief Handles the GAMEOVER phase of the game.
 *
 * @param sockfd The socket file descriptor.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int handleGameover(int sockfd, const char *gameoverLine);

#endif