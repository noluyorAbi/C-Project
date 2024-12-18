#include "./shared_memory.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define MAX_PLAYERS 100 // Definieren Sie eine maximale Anzahl von Spielern

int createSharedMemory(int numPlayers) {
  // Eingabevalidierung
  if (numPlayers < 0 || numPlayers > MAX_PLAYERS) {
    fprintf(
      stderr,
      "Ungültige Anzahl von Spielern: %d. Muss zwischen 0 und %d liegen.\n",
      numPlayers, MAX_PLAYERS);
    return SHM_ERROR_CREATION;
  }

  size_t shmSize = sizeof(SharedMemory) + numPlayers * sizeof(Player);
  int shmid = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | SHM_PERMISSIONS);
  if (shmid == -1) {
    perror("shmget fehlgeschlagen");
    return SHM_ERROR_CREATION;
  }
  return shmid;
}

SharedMemory *attachSharedMemory(int shmid) {
  void *shmaddr = shmat(shmid, NULL, 0);
  if (shmaddr == (void *) -1) {
    perror("shmat fehlgeschlagen");
    return NULL;
  }
  return (SharedMemory *) shmaddr;
}

int detachSharedMemory(SharedMemory *shm) {
  if (shm == NULL) {
    fprintf(stderr, "Versuch, einen NULL-Pointer abzutrennen.\n");
    return SHM_ERROR_DETACH;
  }
  if (shmdt((void *) shm) == -1) {
    perror("shmdt fehlgeschlagen");
    return SHM_ERROR_DETACH;
  }
  return SHM_SUCCESS;
}

int removeSharedMemory(int shmid) {
  if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    perror("shmctl(IPC_RMID) fehlgeschlagen");
    return SHM_ERROR_REMOVE;
  }
  return SHM_SUCCESS;
}

int initSharedMemory(int numPlayers, const char *gameName, int playerNumber,
                     pid_t thinkerPID, pid_t connectorPID) {
  // Überprüfen Sie, ob gameName NULL ist
  if (gameName == NULL) {
    fprintf(stderr, "Spielname darf nicht NULL sein.\n");
    return SHM_ERROR_CREATION;
  }

  // Überprüfen Sie die Länge von gameName
  if (strlen(gameName) >= sizeof(((SharedMemory *) 0)->gameName)) {
    fprintf(
      stderr,
      "Spielname zu lang: %zu Zeichen. Maximal erlaubt sind %zu Zeichen.\n",
      strlen(gameName), sizeof(((SharedMemory *) 0)->gameName) - 1);
    return SHM_ERROR_CREATION;
  }

  int shmid = createSharedMemory(numPlayers);
  if (shmid < 0) {
    return shmid; // Fehlercode von createSharedMemory weitergeben
  }

  SharedMemory *shm = attachSharedMemory(shmid);
  if (shm == NULL) {
    return SHM_ERROR_ATTACH;
  }

  // Initialisiere die SharedMemory-Struktur
  strncpy(shm->gameName, gameName, sizeof(shm->gameName) - 1);
  shm->gameName[sizeof(shm->gameName) - 1] =
    '\0'; // Nullterminierung sicherstellen
  shm->playerNumber = playerNumber;
  shm->playerCount = numPlayers;
  shm->thinkerPID = thinkerPID;
  shm->connectorPID = connectorPID;

  // Initialisiere die Spieler-Arrays (optional, falls notwendig)
  for (int i = 0; i < numPlayers; i++) {
    shm->players[i].playerNumber = 0; // Oder eine andere Standard-Spielernummer
    shm->players[i].playerName[0] = '\0'; // Leerer String
    shm->players[i].isRegistered = false;
  }

  return shmid;
}

void cleanupSharedMemory(int shmid, SharedMemory *shm) {
  if (shm != NULL) {
    if (detachSharedMemory(shm) != SHM_SUCCESS) {
      perror("Shared Memory konnte nicht getrennt werden");
    }
  }

  if (shmid != -1) {
    if (removeSharedMemory(shmid) != SHM_SUCCESS) {
      perror("Shared Memory konnte nicht entfernt werden");
    }
  }
}
