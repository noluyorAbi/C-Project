#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <stddef.h>

/**
 * @brief Processes the WAIT phase of the game, where the server
 *        waits for further instructions or player actions.
 *
 * @param sockfd The file descriptor for the socket used for communication.
 * @param waitLine A string containing the WAIT phase-specific input or
 * commands.
 * @return int Returns EXIT_SUCCESS if the operation completes successfully,
 *             or EXIT_FAILURE if an error occurs.
 */
int handleWait(int sockfd, const char *waitLine);

/**
 * @brief Handles the MOVE phase of the game.
 *
 * @param sockfd The socket file descriptor.
 * @param piece_data Buffer for storing game state data.
 * @param shm Pointer to second SHM segment.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int handleMove(int sockfd, const char *moveLine, char *piece_data, char *shm);

/**
 * @brief Handles the GAMEOVER phase of the game.
 *
 * @param sockfd The socket file descriptor.
 * @param piece_data Buffer for storing game state data.
 * @param shm Pointer to second SHM segment.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int handleGameover(int sockfd, const char *gameoverLine, char *piece_data,
                   char *shm);

#endif