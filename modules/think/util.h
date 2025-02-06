#ifndef C_PROJECT_UTIL_H
#define C_PROJECT_UTIL_H

#include "../shared_memory/shared_memory.h"

#include <stddef.h>

extern int shmid_info;         // ID des neuen "Initial Game Info" SHM-Bereichs
extern SharedMemory *shm_info; // Pointer auf das SHM-Segment mit Spiel-Infos

void place_piece_on_board(char board[], const char *position);
char *find_next_free_spot(const char board[], const int occupiedPositions[]);
int can_form_mill(char board[], int pos, char myPiece);
int is_position_free(const int occupiedPositions[], char row, char col);
const char *index_to_position(int index);
int check_removable(const char board[], char myPiece, char opponentPiece);
int get_board_index(const char *position);
int find_opponent_piece(const char board[], char opponentPiece);
char get_my_symbol(const char *gameState);
void set_player_number(int player);

#endif