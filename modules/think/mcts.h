#ifndef MCTS_H
#define MCTS_H

#include <stddef.h>

// Structure for representing the game state
typedef struct {
  char board[25];    // Game board (25 positions)
  int player_number; // Current player (1 or 2)
} GameState;

// Structure for MCTS nodes
typedef struct MCTSNode {
  GameState state;            // Current game state at this node
  struct MCTSNode *parent;    // Pointer to the parent node
  struct MCTSNode **children; // Array of child nodes
  int num_children;           // Number of child nodes
  int visits;                 // Number of visits to this node
  double wins;                // Total number of wins for this node
} MCTSNode;

// Main MCTS function to decide the best move
char *mcts_decide_move(GameState *state, int time_limit_ms);

// Helper functions for MCTS
MCTSNode *create_node(GameState *state, MCTSNode *parent);
void free_tree(MCTSNode *node);

// Game state utility functions
int get_possible_moves(GameState state, char possible_moves[25][10]);
int is_game_over(GameState *state);
char *get_random_move(GameState *state);
double evaluate_result(GameState *state);
char *get_move_from_state(GameState *old_state, GameState *new_state);

#endif // MCTS_H