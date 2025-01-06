#ifndef THINKER_H
#define THINKER_H

#define SHM_KEY 12345

/**
 * @brief Processes game state from shared memory and writes a move to the pipe.
 * @param pipe_fd The file descriptor of the write-end of the pipe.
 */
void think(int pipe_fd);

/**
 * @brief Signal handler for SIGUSR1 to trigger thinking.
 * @param signum Signal number.
 */
void signal_handler(int signum);

/**
 * @brief Cleans up resources when a termination signal is received.
 * @param signum Signal number.
 */
void signalCleanup(int signum);

#endif // THINKER_H