// shared_memory.c
#include "./shared_memory.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#define MAX_PLAYERS 100

/**
 * Creates a shared memory segment for the specified number of players.
 *
 * @param numPlayers The number of players.
 * @return The shared memory ID on success, or a negative error code on failure.
 */
int createSharedMemory(int numPlayers) {
  // Input validation
  if (numPlayers < 0 || numPlayers > MAX_PLAYERS) {
    fprintf(stderr,
            "[ERROR] Ungültige Anzahl von Spielern: %d. Muss zwischen 0 und %d "
            "liegen.\n",
            numPlayers, MAX_PLAYERS);
    return SHM_ERROR_CREATION;
  }

  size_t shmSize = sizeof(SharedMemory) + numPlayers * sizeof(Player);
  int shmid = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | SHM_PERMISSIONS);
  if (shmid == -1) {
    fprintf(stderr, "[ERROR] shmget fehlgeschlagen: %s\n", strerror(errno));
    return SHM_ERROR_CREATION;
  }
  return shmid;
}

/**
 * Attaches to the shared memory segment with the specified ID.
 *
 * @param shmid The shared memory ID.
 * @return A pointer to the shared memory on success, or NULL on failure.
 */
SharedMemory *attachSharedMemory(int shmid) {
  void *shmaddr = shmat(shmid, NULL, 0);
  if (shmaddr == (void *) -1) {
    fprintf(stderr, "[ERROR] shmat fehlgeschlagen: %s\n", strerror(errno));
    return NULL;
  }
  return (SharedMemory *) shmaddr;
}

/**
 * Detaches the shared memory segment.
 *
 * @param shm Pointer to the shared memory.
 * @return SHM_SUCCESS on success, or a negative error code on failure.
 */
int detachSharedMemory(SharedMemory *shm) {
  if (shm == NULL) {
    fprintf(stderr, "[ERROR] Versuch, einen NULL-Pointer abzutrennen.\n");
    return SHM_ERROR_DETACH;
  }
  if (shmdt((void *) shm) == -1) {
    fprintf(stderr, "[ERROR] shmdt fehlgeschlagen: %s\n", strerror(errno));
    return SHM_ERROR_DETACH;
  }
  return SHM_SUCCESS;
}

/**
 * Removes the shared memory segment.
 *
 * @param shmid The shared memory ID.
 * @return SHM_SUCCESS on success, or a negative error code on failure.
 */
int removeSharedMemory(int shmid) {
  if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    fprintf(stderr, "[ERROR] shmctl(IPC_RMID) fehlgeschlagen: %s\n",
            strerror(errno));
    return SHM_ERROR_REMOVE;
  }
  return SHM_SUCCESS;
}

/**
 * Initializes the shared memory with game and player information.
 *
 * @param numPlayers The number of players.
 * @param gameName The name of the game.
 * @param playerNumber The player number.
 * @param thinkerPID The PID of the thinker process.
 * @param connectorPID The PID of the connector process.
 * @return The shared memory ID on success, or a negative error code on failure.
 */
int initSharedMemory(int numPlayers, const char *gameName, int playerNumber,
                     pid_t thinkerPID, pid_t connectorPID) {
  // Validate if gameName is NULL
  if (gameName == NULL) {
    fprintf(stderr, "[ERROR] Spielname darf nicht NULL sein.\n");
    return SHM_ERROR_CREATION;
  }

  // Validate the length of gameName
  if (strlen(gameName) >= sizeof(((SharedMemory *) 0)->gameName)) {
    fprintf(stderr,
            "[ERROR] Spielname zu lang: %zu Zeichen. Maximal erlaubt sind %zu "
            "Zeichen.\n",
            strlen(gameName), sizeof(((SharedMemory *) 0)->gameName) - 1);
    return SHM_ERROR_CREATION;
  }

  int shmid = createSharedMemory(numPlayers);
  if (shmid < 0) {
    return shmid; // Propagate the error code from createSharedMemory
  }

  SharedMemory *shm = attachSharedMemory(shmid);
  if (shm == NULL) {
    removeSharedMemory(shmid); // Cleanup on failure
    return SHM_ERROR_ATTACH;
  }

  // Initialize the SharedMemory structure
  strncpy(shm->gameName, gameName, sizeof(shm->gameName) - 1);
  shm->gameName[sizeof(shm->gameName) - 1] = '\0'; // Ensure null termination
  shm->playerNumber = playerNumber;
  shm->playerCount = numPlayers;
  shm->thinkerPID = thinkerPID;
  shm->connectorPID = connectorPID;

  // Initialize the players array
  for (int i = 0; i < numPlayers; i++) {
    shm->players[i].playerNumber = 0;     // Default player number
    shm->players[i].playerName[0] = '\0'; // Empty string
    shm->players[i].isRegistered = false;
  }

  return shmid;
}

/**
 * Cleans up the shared memory by detaching and removing it.
 *
 * @param shmid The shared memory ID.
 * @param shm Pointer to the shared memory.
 */
void cleanupSharedMemory(int shmid, SharedMemory *shm) {
  if (shm != NULL) {
    if (detachSharedMemory(shm) != SHM_SUCCESS) {
      fprintf(stderr,
              "[ERROR] Shared Memory konnte nicht getrennt werden: %s\n",
              strerror(errno));
    } else {
      printf("[INFO] Shared Memory erfolgreich getrennt.\n");
    }
  }

  if (shmid != -1) {
    if (removeSharedMemory(shmid) != SHM_SUCCESS) {
      fprintf(stderr,
              "[ERROR] Shared Memory konnte nicht entfernt werden: %s\n",
              strerror(errno));
    } else {
      printf("[INFO] Shared Memory erfolgreich entfernt.\n");
    }
  }
}
