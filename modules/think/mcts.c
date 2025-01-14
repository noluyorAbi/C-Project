#include "mcts.h"

#include "think.h"
#include "util.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Create a new MCTS node
MCTSNode *create_node(GameState *state, MCTSNode *parent) {
  MCTSNode *node = (MCTSNode *) malloc(sizeof(MCTSNode));
  node->state = *state;
  node->parent = parent;
  node->children = NULL;
  node->num_children = 0;
  node->visits = 0;
  node->wins = 0.0;
  return node;
}

// Select the best node using the UCT formula
MCTSNode *select_node(MCTSNode *node) {
  MCTSNode *best_child = NULL;
  double best_uct = -1.0;

  for (int i = 0; i < node->num_children; i++) {
    MCTSNode *child = node->children[i];
    double uct = (child->wins / (child->visits + 1e-6))
                 + sqrt(2.0 * log(node->visits + 1) / (child->visits + 1e-6));
    if (uct > best_uct) {
      best_uct = uct;
      best_child = child;
    }
  }
  return best_child;
}

// Expand a node by adding all possible child nodes
void expand_node(MCTSNode *node) {
  char possible_moves[25][10]; // Array to store possible moves
  int num_moves = get_possible_moves(node->state, possible_moves);

  node->children = (MCTSNode **) malloc(num_moves * sizeof(MCTSNode *));
  for (int i = 0; i < num_moves; i++) {
    GameState new_state = node->state;
    place_piece_on_board(new_state.board, possible_moves[i]);
    node->children[i] = create_node(&new_state, node);
  }
  node->num_children = num_moves;
}

// Simulate a random game from the given state
double simulate_game(GameState *state) {
  GameState simulated_state = *state;
  while (!is_game_over(&simulated_state)) {
    char *random_move = get_random_move(&simulated_state);
    place_piece_on_board(simulated_state.board, random_move);
    free(random_move);
  }
  return evaluate_result(&simulated_state);
}

// Backpropagate simulation results up the tree
void backpropagate(MCTSNode *node, double result) {
  while (node != NULL) {
    node->visits++;
    node->wins += result;
    node = node->parent;
  }
}

// Get the best move based on the highest win rate
char *get_best_move(MCTSNode *root) {
  MCTSNode *best_child = NULL;
  double best_score = -1.0;

  for (int i = 0; i < root->num_children; i++) {
    MCTSNode *child = root->children[i];
    double score = (double) child->wins / (child->visits + 1e-6);
    if (score > best_score) {
      best_score = score;
      best_child = child;
    }
  }

  if (best_child != NULL) {
    return strdup(get_move_from_state(&root->state, &best_child->state));
  }
  return NULL;
}

// Main MCTS function to decide the best move within the time limit
char *mcts_decide_move(GameState *state, int time_limit_ms) {
  clock_t start_time = clock();

  MCTSNode *root = create_node(state, NULL);
  while ((clock() - start_time) * 1000 / CLOCKS_PER_SEC < time_limit_ms) {
    MCTSNode *selected_node = select_node(root);
    if (selected_node->num_children == 0) {
      expand_node(selected_node);
    }
    MCTSNode *node_to_simulate = (selected_node->num_children > 0)
                                   ? selected_node->children[0]
                                   : selected_node;
    double result = simulate_game(&node_to_simulate->state);
    backpropagate(node_to_simulate, result);
  }

  char *best_move = get_best_move(root);
  free_tree(root); // Free memory
  return best_move;
}

// Get all possible moves for the current game state
int get_possible_moves(GameState state, char possible_moves[25][10]) {
  int num_moves = 0;
  for (int i = 0; i < 25; i++) {
    if (state.board[i] == '+') { // Empty position
      snprintf(possible_moves[num_moves], 10, "%d", i);
      num_moves++;
    }
  }
  return num_moves;
}

// Check if the game is over
int is_game_over(GameState *state) {
  // Example: Return 1 if the game is over, 0 otherwise
  return 0;
}

// Get a random valid move
char *get_random_move(GameState *state) {
  char possible_moves[25][10];
  int num_moves = get_possible_moves(*state, possible_moves);
  if (num_moves == 0) {
    return NULL; // No valid moves
  }
  int random_index = rand() % num_moves;
  return strdup(possible_moves[random_index]);
}

// Evaluate the result of the game
double evaluate_result(GameState *state) {
  // Example: Return 1.0 for win, -1.0 for loss, 0 for draw
  return 0.0;
}

// Get the move that resulted in a new state
char *get_move_from_state(GameState *old_state, GameState *new_state) {
  for (int i = 0; i < 25; i++) {
    if (old_state->board[i] != new_state->board[i]) {
      char *move = (char *) malloc(10);
      snprintf(move, 10, "%d", i);
      return move;
    }
  }
  return NULL;
}

// Free the entire MCTS tree
void free_tree(MCTSNode *node) {
  for (int i = 0; i < node->num_children; i++) {
    free_tree(node->children[i]);
  }
  free(node->children);
  free(node);
}