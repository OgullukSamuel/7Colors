#ifndef AGENTS_H
#define AGENTS_H
#include "../head/Gamestate.h"
#include "../head/map_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../head/utilities.h"
#include <math.h>


Color GR0_IA_Random(GameState* state,Color player);

Color GR0_minmax3(GameState* state,Color player);

Color GR0_minmax6(GameState* state,Color player);


float GR0_alpha_beta_minmax(GameState* state, int depth,float alpha,float beta,Color player,int* best_move, float(*heuristique)(GameState*));

Color GR0_Glouton(GameState* state,Color player);

float heuristique_minmax(GameState* state);

float heuristique_mask(GameState* state,int x, int y,Color player);

Color GR0_frontier_IA1(GameState* state, Color player);

Color GR0_frontier_IA5(GameState* state, Color player);

void time_function(const char* function_name, void (*function_to_time)(void));

Color GR0_frontier_IA5_heuristique(GameState* state, Color player);

#endif
