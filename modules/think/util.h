//
// Created by Moritz Dresch on 1/13/25.
//

#ifndef C_PROJECT_UTIL_H
#define C_PROJECT_UTIL_H

void place_piece_on_board(char board[], const char *position);
char *find_next_free_spot(const char board[]);
int is_position_free(const char board[], char row, char col);

#endif // C_PROJECT_UTIL_H
