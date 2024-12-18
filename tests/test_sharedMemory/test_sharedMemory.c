#include "../../modules/shared_memory/shared_memory.h" // Include shared_memory.h directly

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Struktur zur Verwaltung von Teststatistiken
typedef struct {
  int pass_count;
  int fail_count;
} TestStats;

// Struktur zur Bündelung von Shared Memory Testparametern
typedef struct {
  int num_players;
  const char *game_name;
  int player_number;
  pid_t thinker_pid;
  pid_t connector_pid;
} SharedMemoryTestParams;

// Initialisierung der Teststatistik
void init_test_stats(TestStats *stats) {
  stats->pass_count = 0;
  stats->fail_count = 0;
}

// Helper-Funktion zur Berichterstattung von Testergebnissen
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

// Funktion zur Bereinigung von Shared Memory und Fehlerprotokollierung
void cleanup_shared_memory(int shmid, SharedMemory *shm, TestStats *stats,
                           const char *test_name) {
  bool cleanup_passed = true;

  if (shm != NULL) {
    if (detachSharedMemory(shm) != SHM_SUCCESS) {
      printf("[ERROR] %s: Failed to detach shared memory: %s\n", test_name,
             strerror(errno));
      cleanup_passed = false;
    } else {
      printf("[INFO] %s: Shared memory detached successfully.\n", test_name);
    }
  }

  if (shmid != -1) {
    if (removeSharedMemory(shmid) != SHM_SUCCESS) {
      printf("[ERROR] %s: Failed to remove shared memory: %s\n", test_name,
             strerror(errno));
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

// Test: Shared Memory erstellen und anhängen
void test_shared_memory_create_and_attach(TestStats *stats, int num_players) {
  const char *test_name = "SharedMemory erstellen und anhängen";
  printf("\n[Test] %s für %d Spieler\n", test_name, num_players);

  int shmid = createSharedMemory(num_players);
  if (shmid < 0) {
    printf("[ERROR] %s: Failed to create shared memory: %d\n", test_name,
           shmid);
    report_test(stats, test_name, false, "Failed to create shared memory.");
    return;
  }
  printf("[INFO] %s: Shared memory created with shmid: %d\n", test_name, shmid);

  SharedMemory *shm = attachSharedMemory(shmid);
  if (shm == NULL) {
    printf("[ERROR] %s: Failed to attach shared memory.\n", test_name);
    report_test(stats, test_name, false, "Failed to attach shared memory.");
    removeSharedMemory(shmid); // Cleanup bei Fehler
    return;
  }
  printf("[INFO] %s: Shared memory attached at address: %p\n", test_name,
         (void *) shm);

  if (detachSharedMemory(shm) != SHM_SUCCESS) {
    printf("[ERROR] %s: Failed to detach shared memory.\n", test_name);
    report_test(stats, test_name, false, "Failed to detach shared memory.");
    cleanup_shared_memory(shmid, shm, stats, test_name);
    return;
  }
  printf("[INFO] %s: Shared memory detached successfully.\n", test_name);

  if (removeSharedMemory(shmid) != SHM_SUCCESS) {
    printf("[ERROR] %s: Failed to remove shared memory.\n", test_name);
    report_test(stats, test_name, false, "Failed to remove shared memory.");
    return;
  }
  printf("[INFO] %s: Shared memory removed successfully.\n", test_name);

  report_test(stats, test_name, true,
              "Shared memory created, attached, detached und entfernt.");
}

// Funktion zur Überprüfung der Shared Memory Initialisierung
bool verify_shared_memory_initialization(SharedMemory *shm,
                                         SharedMemoryTestParams *params) {
  bool passed = true;

  if (strcmp(shm->gameName, params->game_name) != 0) {
    printf("[ERROR] Game name mismatch: Expected '%s', Got '%s'\n",
           params->game_name, shm->gameName);
    passed = false;
  } else {
    printf("[INFO] Game name initialized correctly.\n");
  }

  if (shm->playerNumber != params->player_number) {
    printf("[ERROR] Player number mismatch: Expected %d, Got %d\n",
           params->player_number, shm->playerNumber);
    passed = false;
  } else {
    printf("[INFO] Player number initialized correctly.\n");
  }

  if (shm->playerCount != params->num_players) {
    printf("[ERROR] Player count mismatch: Expected %d, Got %d\n",
           params->num_players, shm->playerCount);
    passed = false;
  } else {
    printf("[INFO] Player count initialized correctly.\n");
  }

  if (shm->thinkerPID != params->thinker_pid) {
    printf("[ERROR] Thinker PID mismatch: Expected %d, Got %d\n",
           params->thinker_pid, shm->thinkerPID);
    passed = false;
  } else {
    printf("[INFO] Thinker PID initialized correctly.\n");
  }

  if (shm->connectorPID != params->connector_pid) {
    printf("[ERROR] Connector PID mismatch: Expected %d, Got %d\n",
           params->connector_pid, shm->connectorPID);
    passed = false;
  } else {
    printf("[INFO] Connector PID initialized correctly.\n");
  }

  // Überprüfung der Spielern Daten
  for (int i = 0; i < params->num_players; i++) {
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
  }

  return passed;
}

// Test: Shared Memory initialisieren
void test_init_shared_memory(TestStats *stats, SharedMemoryTestParams *params) {
  const char *test_name = "SharedMemory initialisieren";
  printf("\n[Test] %s für %d Spieler\n", test_name, params->num_players);

  int shmid = initSharedMemory(params->num_players, params->game_name,
                               params->player_number, params->thinker_pid,
                               params->connector_pid);
  if (shmid < 0) {
    printf("[ERROR] %s: Failed to initialize shared memory: %d\n", test_name,
           shmid);
    report_test(stats, test_name, false, "Failed to initialize shared memory.");
    return;
  }

  SharedMemory *shm = attachSharedMemory(shmid);
  if (shm == NULL) {
    printf("[ERROR] %s: Failed to attach shared memory after initialization.\n",
           test_name);
    report_test(stats, test_name, false,
                "Failed to attach shared memory after initialization.");
    removeSharedMemory(shmid);
    return;
  }

  // Verifizierung der Initialisierung
  printf("[INFO] %s: Verifying initialization...\n", test_name);
  bool passed = verify_shared_memory_initialization(shm, params);

  if (passed) {
    report_test(stats, test_name, true,
                "Shared memory initialized and verified successfully.");
  } else {
    report_test(stats, test_name, false, "Initialization verification failed.");
  }

  // Cleanup nach dem Test
  cleanup_shared_memory(shmid, shm, stats, test_name);
}

// Test: Ungültige Eingaben
void test_invalid_input(TestStats *stats) {
  const char *test_name = "Ungültige Eingaben";
  printf("\n[Test] %s\n", test_name);

  bool passed = true;

  // Test: createSharedMemory mit negativer Spieleranzahl
  const char *sub_test1 = "createSharedMemory mit -1 Spielern";
  printf("[INFO] %s: Testing createSharedMemory with -1 players...\n",
         sub_test1);
  int shmid = createSharedMemory(-1);
  if (shmid < 0) {
    printf("[INFO] %s: Correctly failed with negative number of players.\n",
           sub_test1);
    report_test(stats, sub_test1, true,
                "createSharedMemory failed as expected with -1 players.");
  } else {
    printf("[ERROR] %s: Should have failed, but returned shmid: %d\n",
           sub_test1, shmid);
    report_test(stats, sub_test1, false,
                "createSharedMemory did not fail with -1 players.");
    passed = false;
  }

  // Test: initSharedMemory mit zu langem Spielnamen
  const char *sub_test2 = "initSharedMemory mit zu langem Spielnamen";
  char long_game_name[300];
  memset(long_game_name, 'A', sizeof(long_game_name) - 1);
  long_game_name[sizeof(long_game_name) - 1] = '\0';

  printf("[INFO] %s: Testing initSharedMemory with too long game name...\n",
         sub_test2);
  SharedMemoryTestParams params = {.num_players = 1,
                                   .game_name = long_game_name,
                                   .player_number = 1,
                                   .thinker_pid = getpid(),
                                   .connector_pid = getpid() + 1};
  int init_shmid =
    initSharedMemory(params.num_players, params.game_name, params.player_number,
                     params.thinker_pid, params.connector_pid);
  if (init_shmid < 0) {
    printf("[INFO] %s: Correctly failed with too long game name.\n", sub_test2);
    report_test(stats, sub_test2, true,
                "initSharedMemory failed as expected with too long game name.");
  } else {
    printf("[ERROR] %s: Should have failed, but returned shmid: %d\n",
           sub_test2, init_shmid);
    report_test(stats, sub_test2, false,
                "initSharedMemory did not fail with too long game name.");
    passed = false;
  }

  if (passed) {
    report_test(stats, test_name, true, "Invalid input tests passed.");
  } else {
    report_test(stats, test_name, false, "Invalid input tests failed.");
  }
}

// Test: Fehlerbehandlung
void test_error_handling(TestStats *stats) {
  const char *test_name = "Fehlerbehandlung";
  printf("\n[Test] %s\n", test_name);

  bool passed = true;

  // Test: attachSharedMemory mit ungültiger ID
  const char *sub_test1 = "attachSharedMemory mit ungültiger ID (-1)";
  printf("[INFO] %s: Testing attachSharedMemory with invalid ID (-1)...\n",
         sub_test1);
  SharedMemory *shm = attachSharedMemory(-1);
  if (shm == NULL) {
    printf("[INFO] %s: Correctly handled invalid shared memory attach.\n",
           sub_test1);
    report_test(stats, sub_test1, true,
                "attachSharedMemory correctly handled invalid ID.");
  } else {
    printf("[ERROR] %s: Should have failed, but returned: %p\n", sub_test1,
           (void *) shm);
    report_test(stats, sub_test1, false,
                "attachSharedMemory did not handle invalid ID correctly.");
    passed = false;
  }

  // Test: detachSharedMemory mit NULL-Pointer
  const char *sub_test2 = "detachSharedMemory mit NULL-Pointer";
  printf("[INFO] %s: Testing detachSharedMemory with NULL pointer...\n",
         sub_test2);
  int detach_result = detachSharedMemory(NULL);
  if (detach_result != SHM_SUCCESS) {
    printf(
      "[INFO] %s: Correctly handled detachSharedMemory with NULL pointer.\n",
      sub_test2);
    report_test(stats, sub_test2, true,
                "detachSharedMemory correctly handled NULL pointer.");
  } else {
    printf("[ERROR] %s: Should have failed, but returned SHM_SUCCESS.\n",
           sub_test2);
    report_test(stats, sub_test2, false,
                "detachSharedMemory did not handle NULL pointer correctly.");
    passed = false;
  }

  if (passed) {
    report_test(stats, test_name, true, "Error handling tests passed.");
  } else {
    report_test(stats, test_name, false, "Error handling tests failed.");
  }
}

#define MAX_PLAYERS_TEST 100

// Test: Randbedingungen
void test_boundary_conditions(TestStats *stats) {
  const char *test_name = "Randbedingungen";
  printf("\n[Test] %s\n", test_name);

  // Test mit null Spielern
  printf("[INFO] %s: Testing with 0 players...\n", test_name);
  SharedMemoryTestParams params_zero = {.num_players = 0,
                                        .game_name = "ZeroPlayerGame",
                                        .player_number = 1,
                                        .thinker_pid = getpid(),
                                        .connector_pid = getpid() + 1};
  test_shared_memory_create_and_attach(stats, params_zero.num_players);
  test_init_shared_memory(stats, &params_zero);

  // Test mit maximaler Spieleranzahl
  printf("[INFO] %s: Testing with %d players...\n", test_name,
         MAX_PLAYERS_TEST);
  SharedMemoryTestParams params_max = {.num_players = MAX_PLAYERS_TEST,
                                       .game_name = "MaxPlayerGame",
                                       .player_number = 1,
                                       .thinker_pid = getpid(),
                                       .connector_pid = getpid() + 1};
  test_shared_memory_create_and_attach(stats, params_max.num_players);
  test_init_shared_memory(stats, &params_max);

  report_test(stats, test_name, true, "Boundary condition tests completed.");
}

int main() {
  printf("Starting Shared Memory Tests\n");

  TestStats stats;
  init_test_stats(&stats);

  // Test Shared Memory erstellen und anhängen
  test_shared_memory_create_and_attach(&stats, 2);

  // Test Shared Memory initialisieren
  SharedMemoryTestParams init_params = {.num_players = 2,
                                        .game_name = "TestGame",
                                        .player_number = 1,
                                        .thinker_pid = getpid(),
                                        .connector_pid = getpid() + 1};
  test_init_shared_memory(&stats, &init_params);

  // Test ungültige Eingaben
  test_invalid_input(&stats);

  // Test Randbedingungen
  test_boundary_conditions(&stats);

  // Test Fehlerbehandlung
  test_error_handling(&stats);

  printf("\nAll tests completed\n");
  printf("[SUMMARY] Bestanden: %d, Fehlgeschlagen: %d\n", stats.pass_count,
         stats.fail_count);
  return (stats.fail_count == 0) ? 0 : 1;
}
