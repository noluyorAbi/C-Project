#include "../../modules/shared_memory/shared_memory.h" // Include shared_memory.h directly

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int PASS_COUNT = 0;
int FAIL_COUNT = 0;

// Helper function to report test results
void report_test(const char *test_name, bool passed, const char *message) {
  if (passed) {
    printf("[PASS] %s: %s\n", test_name, message);
    PASS_COUNT++;
  } else {
    printf("[FAIL] %s: %s\n", test_name, message);
    FAIL_COUNT++;
  }
}

void testSharedMemoryCreateAndAttach(int numPlayers) {
  const char *test_name = "SharedMemory erstellen und anhaengen";
  printf("\n[Test] Creating and attaching shared memory for %d players\n",
         numPlayers);

  int shmid = createSharedMemory(numPlayers);
  if (shmid < 0) {
    printf("[ERROR] Failed to create shared memory: %d\n", shmid);
    report_test(test_name, false, "Failed to create shared memory.");
    return;
  }
  printf("[INFO] Shared memory created with shmid: %d\n", shmid);

  SharedMemory *shm = attachSharedMemory(shmid);
  if (shm == NULL) {
    printf("[ERROR] Failed to attach shared memory.\n");
    report_test(test_name, false, "Failed to attach shared memory.");
    removeSharedMemory(shmid); // Cleanup on failure
    return;
  }
  printf("[INFO] Shared memory attached at address: %p\n", (void *) shm);

  if (detachSharedMemory(shm) != SHM_SUCCESS) {
    printf("[ERROR] Failed to detach shared memory.\n");
    report_test(test_name, false, "Failed to detach shared memory.");
    removeSharedMemory(shmid); // Cleanup
    return;
  }
  printf("[INFO] Shared memory detached successfully.\n");

  if (removeSharedMemory(shmid) != SHM_SUCCESS) {
    printf("[ERROR] Failed to remove shared memory.\n");
    report_test(test_name, false, "Failed to remove shared memory.");
    return;
  }
  printf("[INFO] Shared memory removed successfully.\n");

  report_test(
    test_name, true,
    "Shared memory created, attached, detached, and removed successfully.");
}

void testInitSharedMemory(int numPlayers) {
  const char *test_name = "SharedMemory initialisieren";
  printf("\n[Test] Initializing shared memory for %d players\n", numPlayers);

  const char *gameName = "TestGame";
  int playerNumber = 1;
  pid_t thinkerPID = getpid();
  pid_t connectorPID = getpid() + 1; // Simulate another process

  int shmid = initSharedMemory(numPlayers, gameName, playerNumber, thinkerPID,
                               connectorPID);
  if (shmid < 0) {
    printf("[ERROR] Failed to initialize shared memory: %d\n", shmid);
    report_test(test_name, false, "Failed to initialize shared memory.");
    return;
  }

  SharedMemory *shm = attachSharedMemory(shmid);
  if (shm == NULL) {
    printf("[ERROR] Failed to attach shared memory after initialization.\n");
    report_test(test_name, false,
                "Failed to attach shared memory after initialization.");
    removeSharedMemory(shmid);
    return;
  }

  // Verify initialization
  printf("[INFO] Verifying initialization...\n");
  bool passed = true;

  if (strcmp(shm->gameName, gameName) != 0) {
    printf("[ERROR] Game name mismatch: Expected '%s', Got '%s'\n", gameName,
           shm->gameName);
    passed = false;
  } else {
    printf("[INFO] Game name initialized correctly.\n");
  }

  if (shm->playerNumber != playerNumber) {
    printf("[ERROR] Player number mismatch: Expected %d, Got %d\n",
           playerNumber, shm->playerNumber);
    passed = false;
  } else {
    printf("[INFO] Player number initialized correctly.\n");
  }

  if (shm->playerCount != numPlayers) {
    printf("[ERROR] Player count mismatch: Expected %d, Got %d\n", numPlayers,
           shm->playerCount);
    passed = false;
  } else {
    printf("[INFO] Player count initialized correctly.\n");
  }

  if (shm->thinkerPID != thinkerPID) {
    printf("[ERROR] Thinker PID mismatch: Expected %d, Got %d\n", thinkerPID,
           shm->thinkerPID);
    passed = false;
  } else {
    printf("[INFO] Thinker PID initialized correctly.\n");
  }

  if (shm->connectorPID != connectorPID) {
    printf("[ERROR] Connector PID mismatch: Expected %d, Got %d\n",
           connectorPID, shm->connectorPID);
    passed = false;
  } else {
    printf("[INFO] Connector PID initialized correctly.\n");
  }

  // Verify players data
  for (int i = 0; i < numPlayers; i++) {
    if (shm->players[i].playerNumber != 0) {
      printf("[ERROR] Player %d number mismatch: Expected 0, Got %d\n", i + 1,
             shm->players[i].playerNumber);
      passed = false;
    }
    if (shm->players[i].playerName[0] != '\0') {
      printf(
        "[ERROR] Player %d name mismatch: Expected empty string, Got '%s'\n",
        i + 1, shm->players[i].playerName);
      passed = false;
    }
    if (shm->players[i].isRegistered != false) {
      printf("[ERROR] Player %d registration status mismatch: Expected false, "
             "Got %s\n",
             i + 1, shm->players[i].isRegistered ? "true" : "false");
      passed = false;
    }
  }

  if (passed) {
    printf("[INFO] All initialization verifications passed.\n");
    report_test(test_name, true,
                "Shared memory initialized and verified successfully.");
  } else {
    report_test(test_name, false, "Initialization verification failed.");
  }

  // Cleanup after test
  if (detachSharedMemory(shm) != SHM_SUCCESS) {
    printf("[ERROR] Failed to detach shared memory after initialization.\n");
    report_test(test_name, false,
                "Failed to detach shared memory after initialization.");
  }

  if (removeSharedMemory(shmid) != SHM_SUCCESS) {
    printf("[ERROR] Failed to remove shared memory after initialization.\n");
    report_test(test_name, false,
                "Failed to remove shared memory after initialization.");
  }
}

void testInvalidInput() {
  const char *test_name = "Ungültige Eingaben";
  printf("\n[Test] Testing with invalid inputs\n");

  bool passed = true;

  // Test createSharedMemory with negative number of players
  printf("[INFO] Testing createSharedMemory with -1 players...\n");
  int shmid = createSharedMemory(-1);
  if (shmid < 0) {
    printf("[INFO] createSharedMemory correctly failed with negative number of "
           "players\n");
  } else {
    printf("[ERROR] createSharedMemory should fail with negative number of "
           "players, but returned shmid: %d\n",
           shmid);
    passed = false;
  }

  // Test initSharedMemory with invalid game name (too long)
  char longGameName[300];
  memset(longGameName, 'A', sizeof(longGameName) - 1);
  longGameName[sizeof(longGameName) - 1] = '\0';

  printf("[INFO] Testing initSharedMemory with too long game name...\n");
  int init_shmid = initSharedMemory(1, longGameName, 1, getpid(), getpid() + 1);
  if (init_shmid < 0) {
    printf(
      "[INFO] initSharedMemory correctly failed with too long game name\n");
  } else {
    printf("[ERROR] initSharedMemory should fail with too long game name, but "
           "returned shmid: %d\n",
           init_shmid);
    passed = false;
  }

  if (passed) {
    report_test(test_name, true,
                "createSharedMemory and initSharedMemory failed correctly with "
                "invalid inputs.");
  } else {
    report_test(test_name, false,
                "Invalid input tests did not fail as expected.");
  }
}

void testErrorHandling() {
  const char *test_name = "Fehlerbehandlung";
  printf("\n[Test] Error handling\n");

  bool passed = true;

  // Try attaching to an invalid shared memory ID
  printf("[INFO] Testing attachSharedMemory with invalid ID (-1)...\n");
  SharedMemory *shm = attachSharedMemory(-1);
  if (shm == NULL) {
    printf("[INFO] Correctly handled invalid shared memory attach\n");
  } else {
    printf("[ERROR] attachSharedMemory should fail with invalid ID, but "
           "returned: %p\n",
           (void *) shm);
    passed = false;
  }

  // Try detaching a NULL pointer
  printf("[INFO] Testing detachSharedMemory with NULL pointer...\n");
  int detach_result = detachSharedMemory(NULL);
  if (detach_result != SHM_SUCCESS) {
    printf("[INFO] Correctly handled detachSharedMemory with NULL pointer\n");
  } else {
    printf("[ERROR] detachSharedMemory with NULL pointer should fail, but "
           "returned SHM_SUCCESS\n");
    passed = false;
  }

  if (passed) {
    report_test(test_name, true, "Error handling tests passed.");
  } else {
    report_test(test_name, false, "Error handling tests failed.");
  }
}

#define MAX_PLAYERS_TEST 100

void testBoundaryConditions() {
  const char *test_name = "Boundary conditions";
  printf("\n[Test] Boundary conditions\n");

  // Test with zero players
  printf("[INFO] Testing with 0 players...\n");
  testSharedMemoryCreateAndAttach(0);
  testInitSharedMemory(0);

  // Test with a large number of players
  printf("[INFO] Testing with %d players...\n", MAX_PLAYERS_TEST);
  testSharedMemoryCreateAndAttach(MAX_PLAYERS_TEST);
  testInitSharedMemory(MAX_PLAYERS_TEST);

  report_test(test_name, true, "Boundary condition tests completed.");
}

int main() {
  printf("Starting Shared Memory Tests\n");

  // Test shared memory creation and attach
  testSharedMemoryCreateAndAttach(2);

  // Test shared memory initialization
  testInitSharedMemory(2);

  // Test invalid inputs
  testInvalidInput();

  // Test boundary conditions
  testBoundaryConditions();

  // Test error handling
  testErrorHandling();

  printf("\nAll tests completed\n");
  printf("[SUMMARY] Bestanden: %d, Fehlgeschlagen: %d\n", PASS_COUNT,
         FAIL_COUNT);
  return (FAIL_COUNT == 0) ? 0 : 1;
}
