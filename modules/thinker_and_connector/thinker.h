#ifndef THINKER_H
#define THINKER_H

#include <signal.h>

/**
 * @brief Signal handler for SIGUSR1 to trigger the Thinker logic.
 *
 * @param signum The signal number (SIGUSR1).
 */
void sigusr1_handler(int signum);

#endif // THINKER_H