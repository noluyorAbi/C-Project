#ifndef NINE_MENS_MORRIS_H
#define NINE_MENS_MORRIS_H

#include <stdbool.h>

#define MAX_PIECES 10000
#define MAX_POSITIONS 2500

typedef enum { SET, REMOVE, MOVE, FINISHED } GameAction;

int count_pieces(const char board[], char piece);
bool is_game_over(const char board[], char myPiece, char opponentPiece);
GameAction determine_next_action(int placedPieces, int moveCounter);
void place_piece_on_board(char board[], const char *position);
int is_in_mill(const char board[], int index);
void log_next_action(const char board[], char myPiece, char opponentPiece,
                     int placedPieces);

#endif
