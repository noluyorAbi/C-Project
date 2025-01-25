#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

// Constants for Shared Memory
#define SHM_PERMISSIONS 0666
#define MAX_PLAYERS_TEST 100
#define INITIAL_SIZE 32768

// Error codes for Shared Memory operations
typedef enum {
  SHM_SUCCESS = 0,
  SHM_ERROR_CREATION = -1,
  SHM_ERROR_ATTACH = -2,
  SHM_ERROR_DETACH = -3,
  SHM_ERROR_REMOVE = -4,
} ShmError;

// Structure for player data
typedef struct {
  int playerNumber;
  char playerName[128];
  bool isRegistered;
} Player;

// Structure for data stored in Shared Memory
typedef struct {
  char gameName[256];
  int playerNumber;
  int playerCount;
  pid_t thinkerPID;
  pid_t connectorPID;
  Player players[]; // Flexible array member for players
} SharedMemory;

// Structure for Shared Memory with a flag (Board/Data SHM)
typedef struct {
  int flag; // 1 indicates that a new move should be calculated
  char game_data[INITIAL_SIZE]; // Game state data
} shm_data_t;

// Function declarations
/**
 * @brief Creates a shared memory segment.
 *
 * @param numPlayers The number of players to reserve space for.
 * @return int The ID of the created shared memory segment or an error code
 * (see ShmError).
 */
int createSharedMemory(int numPlayers);

/**
 * @brief Attaches a shared memory segment to the process.
 *
 * @param shmid The ID of the shared memory segment to attach.
 * @return SharedMemory* A pointer to the beginning of the shared memory segment
 * or NULL on error.
 */
SharedMemory *attachSharedMemory(int shmid);

/**
 * @brief Detaches a shared memory segment from the process.
 *
 * @param shm A pointer to the beginning of the shared memory segment.
 * @return int 0 on success, an error code (see ShmError) otherwise.
 */
int detachSharedMemory(SharedMemory *shm);

/**
 * @brief Marks a shared memory segment for deletion.
 *
 * @param shmid The ID of the shared memory segment to delete.
 * @return int 0 on success, an error code (see ShmError) otherwise.
 */
int removeSharedMemory(int shmid);

/**
 * @brief Initializes the shared memory segment and the contained structure.
 *
 * @param numPlayers The number of players.
 * @param gameName The name of the game.
 * @param playerNumber The player number of the current player.
 * @param thinkerPID The PID of the thinker process.
 * @param connectorPID The PID of the connector process.
 * @return int The ID of the shared memory segment or an error code (see
 * ShmError).
 */
int initSharedMemory(int numPlayers, const char *gameName, int playerNumber,
                     pid_t thinkerPID, pid_t connectorPID);

/**
 * @brief Frees a shared memory segment (detaches and removes it).
 *
 * @param shmid ID of the shared memory segment.
 * @param shm Pointer to the shared memory segment.
 */
void cleanupSharedMemory(int shmid, SharedMemory *shm);

/**
 * Cleans up the second SHM segment by detaching and removing it.
 *
 * @param shmid The shared memory ID.
 * @param shm Pointer to the shared memory.
 */
void cleanupSHM(int shmid, shm_data_t *shm);

#endif