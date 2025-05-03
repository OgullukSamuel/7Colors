#ifndef AGENTS_H
#define AGENTS_H
#include "../head/GR0_Gamestate.h"
#include "../head/GR0_map_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../head/GR0_utilities.h"
#include <math.h>


Color GR0_IA_full_Random(GameState* state,Color player);
Color GR0_IA_Random(GameState* state,Color player);

Color GR0_Glouton(GameState* state,Color player);
Color GR0_Glouton_heuristique(GameState* state,Color player);

Color GR0_minmax1(GameState* state,Color player);
Color GR0_minmax2(GameState* state,Color player);
Color GR0_minmax3(GameState* state,Color player);
Color GR0_minmax4(GameState* state,Color player);
Color GR0_minmax5(GameState* state,Color player);
Color GR0_minmax6(GameState* state,Color player);
Color GR0_minmax7(GameState* state,Color player);
Color GR0_minmax8(GameState* state,Color player);
float GR0_minmax8_evaluation(GameState* state,Color player);

float GR0_alpha_beta_minmax(GameState* state, int depth,float alpha,float beta,Color player,int* best_move, float(*heuristique)(GameState*));

float GR0_heuristique_minmax(GameState* state);
float GR0_heuristique_mask(GameState* state,int x, int y,Color player);
float GR0_heuristique_frontier(GameState* state);
float GR0_heuristique_frontier_upgraded(GameState* state);

Color GR0_frontier_IA5(GameState* state, Color player);
Color GR0_frontier_IA5_heuristique(GameState* state, Color player);


void GR0_time_function(const char* function_name, void (*function_to_time)(void));


Color GR0_hegemonique(GameState* state, Color player);
Color GR0_hegemonique_heuristique(GameState* state, Color player);

Color GR0_mixte(GameState* state, Color player);
#endif
