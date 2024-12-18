#include "../../modules/shared_memory/shared_memory.h" // Include shared_memory.h directly

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Structure to manage test statistics
typedef struct {
  int pass_count;
  int fail_count;
} TestStats;

// Structure to bundle shared memory test parameters
typedef struct {
  int num_players;
  const char *game_name;
  int player_number;
  pid_t thinker_pid;
  pid_t connector_pid;
} SharedMemoryTestParams;

// Initialize test statistics
void init_test_stats(TestStats *stats) {
  stats->pass_count = 0;
  stats->fail_count = 0;
}

// Helper function to report test results
void report_test(TestStats *stats, const char *test_name, bool passed,
                 const char *message) {
  if (passed) {
    printf("[PASS] %s: %s\n", test_name, message);
    stats->pass_count++;
  } else {
    printf("[FAIL] %s: %s\n", test_name, message);
    stats->fail_count++;
  }
}

// Function to clean up shared memory and log errors
void cleanup_shared_memory(int shmid, SharedMemory *shm, TestStats *stats,
                           const char *test_name) {
  bool cleanup_passed = true;

  if (shm != NULL) {
    if (detachSharedMemory(shm) != SHM_SUCCESS) {
      fprintf(stderr, "[ERROR] %s: Failed to detach shared memory: %s\n",
              test_name, strerror(errno));
      cleanup_passed = false;
    } else {
      printf("[INFO] %s: Shared memory detached successfully.\n", test_name);
    }
  }

  if (shmid != -1) {
    if (removeSharedMemory(shmid) != SHM_SUCCESS) {
      fprintf(stderr, "[ERROR] %s: Failed to remove shared memory: %s\n",
              test_name, strerror(errno));
      cleanup_passed = false;
    } else {
      printf("[INFO] %s: Shared memory removed successfully.\n", test_name);
    }
  }

  if (cleanup_passed) {
    report_test(stats, test_name, true, "Cleanup erfolgreich.");
  } else {
    report_test(stats, test_name, false, "Fehler bei der Cleanup.");
  }
}

// Test: Create and attach shared memory
void test_shared_memory_create_and_attach(TestStats *stats, int num_players) {
  const char *test_name = "SharedMemory erstellen und anhängen";
  printf("\n[Test] %s für %d Spieler\n", test_name, num_players);

  int shmid = createSharedMemory(num_players);
  if (shmid < 0) {
    fprintf(stderr, "[ERROR] %s: Failed to create shared memory: %d\n",
            test_name, shmid);
    report_test(stats, test_name, false, "Failed to create shared memory.");
    return;
  }
  printf("[INFO] %s: Shared memory created with shmid: %d\n", test_name, shmid);

  SharedMemory *shm = attachSharedMemory(shmid);
  if (shm == NULL) {
    fprintf(stderr, "[ERROR] %s: Failed to attach shared memory.\n", test_name);
    report_test(stats, test_name, false, "Failed to attach shared memory.");
    removeSharedMemory(shmid); // Cleanup on failure
    return;
  }
  printf("[INFO] %s: Shared memory attached at address: %p\n", test_name,
         (void *) shm);

  if (detachSharedMemory(shm) != SHM_SUCCESS) {
    fprintf(stderr, "[ERROR] %s: Failed to detach shared memory.\n", test_name);
    report_test(stats, test_name, false, "Failed to detach shared memory.");
    cleanup_shared_memory(shmid, shm, stats, test_name);
    return;
  }
  printf("[INFO] %s: Shared memory detached successfully.\n", test_name);

  if (removeSharedMemory(shmid) != SHM_SUCCESS) {
    fprintf(stderr, "[ERROR] %s: Failed to remove shared memory.\n", test_name);
    report_test(stats, test_name, false, "Failed to remove shared memory.");
    return;
  }
  printf("[INFO] %s: Shared memory removed successfully.\n", test_name);

  report_test(stats, test_name, true,
              "Shared memory created, attached, detached und entfernt.");
}

// Function to verify shared memory initialization
bool verify_shared_memory_initialization(SharedMemory *shm,
                                         SharedMemoryTestParams *params) {
  bool passed = true;

  if (strcmp(shm->gameName, params->game_name) != 0) {
    fprintf(stderr, "[ERROR] Game name mismatch: Expected '%s', Got '%s'\n",
            params->game_name, shm->gameName);
    passed = false;
  } else {
    printf("[INFO] Game name initialized correctly.\n");
  }

  if (shm->playerNumber != params->player_number) {
    fprintf(stderr, "[ERROR] Player number mismatch: Expected %d, Got %d\n",
            params->player_number, shm->playerNumber);
    passed = false;
  } else {
    printf("[INFO] Player number initialized correctly.\n");
  }

  if (shm->playerCount != params->num_players) {
    fprintf(stderr, "[ERROR] Player count mismatch: Expected %d, Got %d\n",
            params->num_players, shm->playerCount);
    passed = false;
  } else {
    printf("[INFO] Player count initialized correctly.\n");
  }

  if (shm->thinkerPID != params->thinker_pid) {
    fprintf(stderr, "[ERROR] Thinker PID mismatch: Expected %d, Got %d\n",
            params->thinker_pid, shm->thinkerPID);
    passed = false;
  } else {
    printf("[INFO] Thinker PID initialized correctly.\n");
  }

  if (shm->connectorPID != params->connector_pid) {
    fprintf(stderr, "[ERROR] Connector PID mismatch: Expected %d, Got %d\n",
            params->connector_pid, shm->connectorPID);
    passed = false;
  } else {
    printf("[INFO] Connector PID initialized correctly.\n");
  }

  // Check player data
  for (int i = 0; i < params->num_players; i++) {
    if (shm->players[i].playerNumber != 0) {
      fprintf(stderr, "[ERROR] Player %d number mismatch: Expected 0, Got %d\n",
              i + 1, shm->players[i].playerNumber);
      passed = false;
    }
    if (shm->players[i].playerName[0] != '\0') {
      fprintf(
        stderr,
        "[ERROR] Player %d name mismatch: Expected empty string, Got '%s'\n",
        i + 1, shm->players[i].playerName);
      passed = false;
    }
    if (shm->players[i].isRegistered != false) {
      fprintf(stderr,
              "[ERROR] Player %d registration status mismatch: Expected false, "
              "Got %s\n",
              i + 1, shm->players[i].isRegistered ? "true" : "false");
      passed = false;
    }
  }

  if (passed) {
    printf("[INFO] All initialization verifications passed.\n");
  }

  return passed;
}

// Test: Initialize shared memory
void test_init_shared_memory(TestStats *stats, SharedMemoryTestParams *params) {
  const char *test_name = "SharedMemory initialisieren";
  printf("\n[Test] %s für %d Spieler\n", test_name, params->num_players);

  int shmid = initSharedMemory(params->num_players, params->game_name,
                               params->player_number, params->thinker_pid,
                               params->connector_pid);
  if (shmid < 0) {
    fprintf(stderr, "[ERROR] %s: Failed to initialize shared memory: %d\n",
            test_name, shmid);
    report_test(stats, test_name, false, "Failed to initialize shared memory.");
    return;
  }

  SharedMemory *shm = attachSharedMemory(shmid);
  if (shm == NULL) {
    fprintf(
      stderr,
      "[ERROR] %s: Failed to attach shared memory after initialization.\n",
      test_name);
    report_test(stats, test_name, false,
                "Failed to attach shared memory after initialization.");
    removeSharedMemory(shmid);
    return;
  }

  // Verify initialization
  printf("[INFO] %s: Verifying initialization...\n", test_name);
  bool passed = verify_shared_memory_initialization(shm, params);

  if (passed) {
    report_test(stats, test_name, true,
                "Shared memory initialized and verified successfully.");
  } else {
    report_test(stats, test_name, false, "Initialization verification failed.");
  }

  // Cleanup after the test
  cleanup_shared_memory(shmid, shm, stats, test_name);
}

#define MAX_PLAYERS_TEST 100

// Main function
int main() {
  printf("Starting Shared Memory Tests\n");

  TestStats stats;
  init_test_stats(&stats);

  // Test: Create and attach shared memory
  test_shared_memory_create_and_attach(&stats, 2);

  // Test: Initialize shared memory
  SharedMemoryTestParams init_params = {.num_players = 2,
                                        .game_name = "TestGame",
                                        .player_number = 1,
                                        .thinker_pid = getpid(),
                                        .connector_pid = getpid() + 1};
  test_init_shared_memory(&stats, &init_params);

  printf("\nAll tests completed\n");
  printf("[SUMMARY] Bestanden: %d, Fehlgeschlagen: %d\n", stats.pass_count,
         stats.fail_count);
  return (stats.fail_count == 0) ? 0 : 1;
}
