#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <stddef.h>

extern char *shm; // Pointer to second SHM segment

/**
 * @brief Handles the WAIT Phase of the game.
 *
 * @param sockfd The socket file descriptor.
 * @param waitLine A string containing the WAIT phase-specific input or
 * commands.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int handleWait(int sockfd, const char *waitLine);

/**
 * @brief Handles the MOVE phase of the game.
 *
 * @param sockfd The socket file descriptor.
 * @param moveLine A string containing the MOVE phase-specific input or
 * commands.
 * @param piece_data Buffer for storing game state.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int handleMove(int sockfd, const char *moveLine, char *piece_data);

/**
 * @brief Handles the GAMEOVER phase of the game.
 *
 * @param sockfd The socket file descriptor.
 * @param gameoverLine A string containing the GAMEOVER phase-specific input or
 * commands.
 * @param piece_data Buffer for storing game state.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int handleGameover(int sockfd, const char *gameoverLine, char *piece_data);

#endif