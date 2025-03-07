#ifndef AGENTS_H
#define AGENTS_H
#include "../head/Gamestate.h"
#include <stdint.h>
#include <math.h>



Color GR0_IA_Random(GameState* state,Color player);


Color GR0_Agent_MinMax(GameState* state,Color player);

Color GR0_Glouton(GameState* state,Color player);

float heuristique(GameState* state,Color player);

float heuristique_mask(GameState* state,Color player,int x, int y);

float tanh_approx(float x);
#endif
