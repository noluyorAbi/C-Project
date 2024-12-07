#include "../../modules/shared_memory/shared_memory.c"

#include <stdio.h>
#include <string.h>
#include <unistd.h> // For getpid()

// Declare functions from shared_memory.c
int createSharedMemory(int numPlayers);
SharedMemory *attachSharedMemory(int shmid);
int detachSharedMemory(SharedMemory *shm);
int removeSharedMemory(int shmid);

void testSharedMemoryInitialization(int numPlayers) {
  printf("\n[Test] Creating and initializing shared memory for %d players\n",
         numPlayers);

  int shmid = createSharedMemory(numPlayers);
  if (shmid == -1) {
    fprintf(stderr, "Failed to create shared memory\n");
    return;
  }

  SharedMemory *shm = attachSharedMemory(shmid);
  if (shm == NULL) {
    fprintf(stderr, "Failed to attach shared memory\n");
    return;
  }

  // Initialize shared memory
  strcpy(shm->gameName, "ExampleGame");
  shm->playerNumber = 1; // Example player number
  shm->playerCount = numPlayers;
  shm->thinkerPID = getpid();
  shm->connectorPID = getpid();

  // Initialize players
  for (int i = 0; i < numPlayers; i++) {
    shm->players[i].playerNumber = i + 1;
    sprintf(shm->players[i].playerName, "Player%d", i + 1);
    shm->players[i].isRegistered = (i % 2 == 0); // Register every second player
  }

  // Display player info
  for (int i = 0; i < numPlayers; i++) {
    printf("Player %d: %s, Registered: %s\n", shm->players[i].playerNumber,
           shm->players[i].playerName,
           shm->players[i].isRegistered ? "Yes" : "No");
  }

  // Detach from shared memory
  if (detachSharedMemory(shm) == -1) {
    fprintf(stderr, "Failed to detach shared memory\n");
  }

  // Remove shared memory segment
  if (removeSharedMemory(shmid) == -1) {
    fprintf(stderr, "Failed to remove shared memory\n");
  }
}

void testErrorHandling() {
  printf("\n[Test] Error handling\n");

  // Try attaching to an invalid shared memory ID
  int invalidShmid = -1;
  SharedMemory *shm = attachSharedMemory(invalidShmid);
  if (shm == NULL) {
    printf("Correctly handled invalid shared memory attach\n");
  } else {
    fprintf(stderr, "Error: Invalid shared memory attach succeeded\n");
  }

  // Try detaching an invalid shared memory pointer
  if (detachSharedMemory((SharedMemory *) -1) == -1) {
    printf("Correctly handled invalid shared memory detach\n");
  } else {
    fprintf(stderr, "Error: Invalid shared memory detach succeeded\n");
  }
}

void testBoundaryConditions() {
  printf("\n[Test] Boundary conditions\n");

  // Test with zero players
  testSharedMemoryInitialization(0);

  // Test with a large number of players
  testSharedMemoryInitialization(100);
}

int main() {
  // Test with normal number of players
  testSharedMemoryInitialization(2);

  // Test error handling
  testErrorHandling();

  // Test boundary conditions
  testBoundaryConditions();

  printf("\nAll tests completed\n");
  return 0;
}