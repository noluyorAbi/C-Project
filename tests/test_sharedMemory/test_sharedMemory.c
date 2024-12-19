// test_sharedMemory.c
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
              "Shared memory erstellt, angehängt, getrennt und entfernt.");
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

// Test: SharedMemory creation with various num_players values
void test_shared_memory_various_num_players(TestStats *stats) {
  const int test_values[] = {0, 1, MAX_PLAYERS_TEST, MAX_PLAYERS_TEST + 1, -1};
  const int num_tests = sizeof(test_values) / sizeof(test_values[0]);

  for (int i = 0; i < num_tests; i++) {
    int num_players = test_values[i];
    char test_name[100];
    snprintf(test_name, sizeof(test_name), "SharedMemory num_players = %d",
             num_players);
    printf("\n[Test] %s\n", test_name);

    int shmid = createSharedMemory(num_players);
    bool expected_success =
      (num_players > 0 && num_players <= MAX_PLAYERS_TEST);
    if (!expected_success && shmid >= 0) {
      // Unexpected success
      fprintf(stderr,
              "[ERROR] %s: Expected failure but Shared Memory was created with "
              "shmid: %d\n",
              test_name, shmid);
      report_test(stats, test_name, false,
                  "Unexpected success in creating SHM.");
      removeSharedMemory(shmid); // Cleanup
      continue;
    } else if (expected_success && shmid < 0) {
      // Expected success but failed
      fprintf(stderr,
              "[ERROR] %s: Expected success but failed to create Shared "
              "Memory. Error code: %d\n",
              test_name, shmid);
      report_test(stats, test_name, false,
                  "Failed to create SHM when it should succeed.");
      continue;
    }

    if (expected_success) {
      // Attach and then clean up
      SharedMemory *shm = attachSharedMemory(shmid);
      if (shm == NULL) {
        fprintf(stderr, "[ERROR] %s: Failed to attach Shared Memory.\n",
                test_name);
        report_test(stats, test_name, false, "Failed to attach SHM.");
        removeSharedMemory(shmid);
        continue;
      }
      printf("[INFO] %s: Shared memory attached at address: %p\n", test_name,
             (void *) shm);

      // Cleanup
      if (detachSharedMemory(shm) != SHM_SUCCESS) {
        fprintf(stderr, "[ERROR] %s: Failed to detach Shared Memory.\n",
                test_name);
        report_test(stats, test_name, false, "Failed to detach SHM.");
        cleanup_shared_memory(shmid, shm, stats, test_name);
        continue;
      }
      printf("[INFO] %s: Shared memory detached successfully.\n", test_name);

      if (removeSharedMemory(shmid) != SHM_SUCCESS) {
        fprintf(stderr, "[ERROR] %s: Failed to remove Shared Memory.\n",
                test_name);
        report_test(stats, test_name, false, "Failed to remove SHM.");
        continue;
      }
      printf("[INFO] %s: Shared memory removed successfully.\n", test_name);

      report_test(stats, test_name, true,
                  "Shared Memory erstellt, angehängt, getrennt und entfernt.");
    } else {
      // Expected failure, already handled above
      report_test(stats, test_name, true,
                  "Shared Memory Erstellung korrekt fehlgeschlagen.");
    }
  }
}

// Test: Initialize shared memory with invalid parameters
void test_init_shared_memory_invalid_params(TestStats *stats) {
  const char *test_name_null_game =
    "SharedMemory initialisieren mit NULL game_name";
  printf("\n[Test] %s\n", test_name_null_game);

  SharedMemoryTestParams params_null_game = {.num_players = 2,
                                             .game_name = NULL, // Invalid
                                             .player_number = 1,
                                             .thinker_pid = getpid(),
                                             .connector_pid = getpid() + 1};
  int shmid = initSharedMemory(
    params_null_game.num_players, params_null_game.game_name,
    params_null_game.player_number, params_null_game.thinker_pid,
    params_null_game.connector_pid);
  if (shmid < 0) {
    report_test(stats, test_name_null_game, true,
                "Shared Memory Initialization korrekt fehlgeschlagen mit NULL "
                "game_name.");
  } else {
    fprintf(stderr,
            "[ERROR] %s: Shared Memory wurde unerwartet mit NULL game_name "
            "initialisiert. shmid: %d\n",
            test_name_null_game, shmid);
    report_test(
      stats, test_name_null_game, false,
      "Shared Memory wurde unerwartet mit NULL game_name initialisiert.");
    removeSharedMemory(shmid); // Cleanup
  }

  const char *test_name_long_game =
    "SharedMemory initialisieren mit zu langem game_name";
  printf("\n[Test] %s\n", test_name_long_game);

  // Assuming gameName has a maximum size, let's create a string longer than
  // that To prevent buffer overflow, calculate the maximum size based on
  // SharedMemory structure We need to know the size of shm->gameName Assuming
  // shared_memory.h defines SharedMemory and its fields, including gameName So,
  // we can calculate the size as sizeof(((SharedMemory *)0)->gameName) Since
  // sizeof cannot be applied to incomplete types, this code assumes
  // SharedMemory is defined

  size_t game_name_max_size = sizeof(((SharedMemory *) 0)->gameName);
  char long_game_name[game_name_max_size + 10];
  memset(long_game_name, 'A', sizeof(long_game_name) - 1);
  long_game_name[sizeof(long_game_name) - 1] = '\0';

  SharedMemoryTestParams params_long_game = {.num_players = 2,
                                             .game_name =
                                               long_game_name, // Too long
                                             .player_number = 1,
                                             .thinker_pid = getpid(),
                                             .connector_pid = getpid() + 1};
  shmid = initSharedMemory(
    params_long_game.num_players, params_long_game.game_name,
    params_long_game.player_number, params_long_game.thinker_pid,
    params_long_game.connector_pid);
  if (shmid < 0) {
    report_test(stats, test_name_long_game, true,
                "Shared Memory Initialization korrekt fehlgeschlagen mit zu "
                "langem game_name.");
  } else {
    fprintf(stderr,
            "[ERROR] %s: Shared Memory wurde unerwartet mit zu langem "
            "game_name initialisiert. shmid: %d\n",
            test_name_long_game, shmid);
    report_test(
      stats, test_name_long_game, false,
      "Shared Memory wurde unerwartet mit zu langem game_name initialisiert.");
    removeSharedMemory(shmid); // Cleanup
  }
}

// Test: Multiple Shared Memory creations and deletions
void test_shared_memory_multiple_creations(TestStats *stats) {
  const char *test_name = "Multiple SharedMemory Erstellung und Löschung";
  printf("\n[Test] %s\n", test_name);

  int num_iterations = 10;
  bool all_passed = true;

  for (int i = 0; i < num_iterations; i++) {
    printf("\n[%s] Iteration %d/%d\n", test_name, i + 1, num_iterations);
    int shmid = createSharedMemory(2);
    if (shmid < 0) {
      fprintf(stderr,
              "[ERROR] %s: Iteration %d: Failed to create Shared Memory: %d\n",
              test_name, i + 1, shmid);
      report_test(stats, test_name, false,
                  "Failed to create SHM in multiple creation test.");
      all_passed = false;
      continue;
    }
    printf("[INFO] %s: Iteration %d: Shared Memory created with shmid: %d\n",
           test_name, i + 1, shmid);

    SharedMemory *shm = attachSharedMemory(shmid);
    if (shm == NULL) {
      fprintf(stderr,
              "[ERROR] %s: Iteration %d: Failed to attach Shared Memory.\n",
              test_name, i + 1);
      report_test(stats, test_name, false,
                  "Failed to attach SHM in multiple creation test.");
      removeSharedMemory(shmid); // Cleanup
      all_passed = false;
      continue;
    }
    printf("[INFO] %s: Iteration %d: Shared Memory attached at address: %p\n",
           test_name, i + 1, (void *) shm);

    if (detachSharedMemory(shm) != SHM_SUCCESS) {
      fprintf(stderr,
              "[ERROR] %s: Iteration %d: Failed to detach Shared Memory.\n",
              test_name, i + 1);
      report_test(stats, test_name, false,
                  "Failed to detach SHM in multiple creation test.");
      cleanup_shared_memory(shmid, shm, stats, test_name);
      all_passed = false;
      continue;
    }
    printf("[INFO] %s: Iteration %d: Shared Memory detached successfully.\n",
           test_name, i + 1);

    if (removeSharedMemory(shmid) != SHM_SUCCESS) {
      fprintf(stderr,
              "[ERROR] %s: Iteration %d: Failed to remove Shared Memory.\n",
              test_name, i + 1);
      report_test(stats, test_name, false,
                  "Failed to remove SHM in multiple creation test.");
      continue;
    }
    printf("[INFO] %s: Iteration %d: Shared Memory removed successfully.\n",
           test_name, i + 1);

    report_test(stats, test_name, true,
                "Shared Memory erstellt, angehängt, getrennt und entfernt.");
  }

  if (all_passed) {
    report_test(stats, test_name, true,
                "All multiple creation iterations passed successfully.");
  } else {
    report_test(stats, test_name, false,
                "Some multiple creation iterations failed.");
  }
}

// Main function
int main() {
  printf("Starting Shared Memory Tests\n");

  TestStats stats;
  init_test_stats(&stats);

  // Test: Create and attach shared memory with 2 players
  test_shared_memory_create_and_attach(&stats, 2);

  // Test: Initialize shared memory with valid parameters
  SharedMemoryTestParams init_params = {.num_players = 2,
                                        .game_name = "TestGame",
                                        .player_number = 1,
                                        .thinker_pid = getpid(),
                                        .connector_pid = getpid() + 1};
  test_init_shared_memory(&stats, &init_params);

  // Test: SharedMemory creation with various num_players values
  test_shared_memory_various_num_players(&stats);

  // Test: Initialize shared memory with invalid parameters
  test_init_shared_memory_invalid_params(&stats);

  // Test: Multiple SharedMemory creations and deletions
  test_shared_memory_multiple_creations(&stats);

  printf("\nAll tests completed\n");
  printf("[SUMMARY] Bestanden: %d, Fehlgeschlagen: %d\n", stats.pass_count,
         stats.fail_count);
  return (stats.fail_count == 0) ? 0 : 1;
}
