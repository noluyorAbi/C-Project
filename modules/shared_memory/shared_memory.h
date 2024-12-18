#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

// Konstanten für Shared Memory
#define SHM_PERMISSIONS 0666

// Fehlercodes für Shared Memory Operationen
typedef enum {
  SHM_SUCCESS = 0,
  SHM_ERROR_CREATION = -1,
  SHM_ERROR_ATTACH = -2,
  SHM_ERROR_DETACH = -3,
  SHM_ERROR_REMOVE = -4,
} ShmError;

// Struktur für Spielerdaten
typedef struct {
  int playerNumber;
  char playerName[128];
  bool isRegistered;
} Player;

// Struktur für die im Shared Memory abgelegten Daten
typedef struct {
  char gameName[256];
  int playerNumber;
  int playerCount;
  pid_t thinkerPID;
  pid_t connectorPID;
  Player players[]; // Flexibles Array-Mitglied für die Spieler
} SharedMemory;

// Funktionsdeklarationen
/**
 * @brief Erstellt ein Shared-Memory-Segment.
 *
 * @param numPlayers Die Anzahl der Spieler, für die Platz reserviert werden
 * soll.
 * @return int Die ID des erstellten Shared-Memory-Segments oder ein Fehlercode
 * (siehe ShmError).
 */
int createSharedMemory(int numPlayers);

/**
 * @brief Hängt ein Shared-Memory-Segment an den Prozess an.
 *
 * @param shmid Die ID des anzuhängenden Shared-Memory-Segments.
 * @return SharedMemory* Ein Zeiger auf den Anfang des Shared-Memory-Segments
 * oder NULL bei einem Fehler.
 */
SharedMemory *attachSharedMemory(int shmid);

/**
 * @brief Trennt ein Shared-Memory-Segment vom Prozess.
 *
 * @param shm Ein Zeiger auf den Anfang des Shared-Memory-Segments.
 * @return int 0 bei Erfolg, ein Fehlercode (siehe ShmError) sonst.
 */
int detachSharedMemory(SharedMemory *shm);

/**
 * @brief Markiert ein Shared-Memory-Segment zum Löschen.
 *
 * @param shmid Die ID des zu löschenden Shared-Memory-Segments.
 * @return int 0 bei Erfolg, ein Fehlercode (siehe ShmError) sonst.
 */
int removeSharedMemory(int shmid);

/**
 * @brief Initialisiert das Shared-Memory-Segment und die darin enthaltene
 * Struktur.
 *
 * @param numPlayers Die Anzahl der Spieler.
 * @param gameName Der Name des Spiels.
 * @param playerNumber Die Spielernummer des eigenen Spielers.
 * @param thinkerPID Die PID des Thinker-Prozesses.
 * @param connectorPID Die PID des Connector-Prozesses.
 * @return int Die ID des Shared-Memory-Segments oder ein Fehlercode (siehe
 * ShmError).
 */
int initSharedMemory(int numPlayers, const char *gameName, int playerNumber,
                     pid_t thinkerPID, pid_t connectorPID);

/**
 * @brief Gibt ein Shared Memory Segment frei (detached und removed es).
 *
 * @param shmid ID des Shared Memory Segments.
 * @param shm Pointer auf das Shared Memory Segment.
 */
void cleanupSharedMemory(int shmid, SharedMemory *shm);

#endif // SHARED_MEMORY_H