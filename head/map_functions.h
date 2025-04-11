#ifndef MAP_FUNCTIONS_H
#define MAP_FUNCTIONS_H
#include "../head/GameState.h"
#include "../head/queue.h"
#include "../head/utilities.h"

void GR0_free_state(GameState* state);

void GR0_get_network(GameState* state,int *pos , Queue* explored,Queue* coup);

void GR0_get_adjacent_cases(GameState* state, int x, int y, Queue* unexplored, Queue* explored, int SameColor, Queue* movements);

uint8_t GR0_get_move_available(GameState* state,Color player,Queue moves[7]);

struct GameState GR0_virtual_depth_step(GameState* state ,Queue* coup ,int player);

GameState GR0_copy_game_state(GameState* original);

int GR0_partie_finie(GameState* state);

void GR0_step(GameState* state ,Queue* coup ,int player);

int GR0_virtual_glouton_step(GameState* state ,Queue* coup ,int player);


#endif