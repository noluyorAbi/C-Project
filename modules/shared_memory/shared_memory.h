#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h> // Include this to define 'bool'
#include <stddef.h>
#include <sys/types.h>

typedef struct {
  int playerNumber;
  char playerName[128];
  bool isRegistered;
} Player;

typedef struct {
  char gameName[256];
  int playerNumber;
  int playerCount;
  pid_t thinkerPID;
  pid_t connectorPID;
  // Use a flexible array member for players
  Player players[];
} SharedMemory;

#endif // SHARED_MEMORY_H
