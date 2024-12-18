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

// Funktion zur Berechnung der Shared Memory Größe
static size_t calculate_shm_size(int numPlayers) {
  return sizeof(SharedMemory) + numPlayers * sizeof(Player);
}

/**
 * Erstellt ein Shared Memory Segment für die angegebene Anzahl von Spielern.
 *
 * @param numPlayers Die Anzahl der Spieler.
 * @return Der Shared Memory ID bei Erfolg, oder ein negativer Fehlercode bei
 * Fehler.
 */
int createSharedMemory(int numPlayers) {
  // Eingabevalidierung
  if (numPlayers < 0 || numPlayers > MAX_PLAYERS) {
    fprintf(stderr,
            "[ERROR] Ungültige Anzahl von Spielern: %d. Muss zwischen 0 und %d "
            "liegen.\n",
            numPlayers, MAX_PLAYERS);
    return SHM_ERROR_CREATION;
  }

  size_t shmSize = calculate_shm_size(numPlayers);
  int shmid = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | SHM_PERMISSIONS);
  if (shmid == -1) {
    fprintf(stderr, "[ERROR] shmget fehlgeschlagen: %s\n", strerror(errno));
    return SHM_ERROR_CREATION;
  }
  return shmid;
}

/**
 * Hängt an das Shared Memory Segment mit der angegebenen ID an.
 *
 * @param shmid Die Shared Memory ID.
 * @return Ein Zeiger auf das Shared Memory bei Erfolg, oder NULL bei Fehler.
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
 * Trennt das Shared Memory Segment ab.
 *
 * @param shm Zeiger auf das Shared Memory.
 * @return SHM_SUCCESS bei Erfolg, oder ein negativer Fehlercode bei Fehler.
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
 * Entfernt das Shared Memory Segment.
 *
 * @param shmid Die Shared Memory ID.
 * @return SHM_SUCCESS bei Erfolg, oder ein negativer Fehlercode bei Fehler.
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
 * Initialisiert das Shared Memory mit Spiel- und Spielerinformationen.
 *
 * @param numPlayers Die Anzahl der Spieler.
 * @param gameName Der Name des Spiels.
 * @param playerNumber Die Nummer des Spielers.
 * @param thinkerPID Die PID des Denkprozesses.
 * @param connectorPID Die PID des Verbindungsprozesses.
 * @return Die Shared Memory ID bei Erfolg, oder ein negativer Fehlercode bei
 * Fehler.
 */
int initSharedMemory(int numPlayers, const char *gameName, int playerNumber,
                     pid_t thinkerPID, pid_t connectorPID) {
  // Überprüfen, ob gameName NULL ist
  if (gameName == NULL) {
    fprintf(stderr, "[ERROR] Spielname darf nicht NULL sein.\n");
    return SHM_ERROR_CREATION;
  }

  // Überprüfen der Länge von gameName
  if (strlen(gameName) >= sizeof(((SharedMemory *) 0)->gameName)) {
    fprintf(stderr,
            "[ERROR] Spielname zu lang: %zu Zeichen. Maximal erlaubt sind %zu "
            "Zeichen.\n",
            strlen(gameName), sizeof(((SharedMemory *) 0)->gameName) - 1);
    return SHM_ERROR_CREATION;
  }

  int shmid = createSharedMemory(numPlayers);
  if (shmid < 0) {
    return shmid; // Fehlercode von createSharedMemory weitergeben
  }

  SharedMemory *shm = attachSharedMemory(shmid);
  if (shm == NULL) {
    removeSharedMemory(shmid); // Cleanup bei Fehler
    return SHM_ERROR_ATTACH;
  }

  // Initialisieren der SharedMemory Struktur
  strncpy(shm->gameName, gameName, sizeof(shm->gameName) - 1);
  shm->gameName[sizeof(shm->gameName) - 1] =
    '\0'; // Sicherstellen der Null-Terminierung
  shm->playerNumber = playerNumber;
  shm->playerCount = numPlayers;
  shm->thinkerPID = thinkerPID;
  shm->connectorPID = connectorPID;

  // Initialisieren des players Arrays
  for (int i = 0; i < numPlayers; i++) {
    shm->players[i].playerNumber = 0; // Oder eine andere Standardspieler-Nummer
    shm->players[i].playerName[0] = '\0'; // Leerer String
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
