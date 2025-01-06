#ifndef THINK_H
#define THINK_H

#include <stddef.h>

extern int pipe_fd[2]; // Pipe for the interprocess-communication.

/**
 * @brief Calculates the next move.
 *
 * @param Pointer to game state in second SHM segment.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int think(char *gameState);

#endif // THINK_H