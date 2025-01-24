#ifndef C_PROJECT_UTIL_H
#define C_PROJECT_UTIL_H

void place_piece_on_board(char board[], const char *position);
char *find_next_free_spot(const char board[], const int occupiedPositions[]);
int can_form_mill(char board[], int pos, char myPiece);
int is_position_free(const int occupiedPositions[], char row, char col);
const char *index_to_position(int index);
int check_removable(const char board[], char myPiece, char opponentPiece);

#endif