#ifndef AGENTS_H
#define AGENTS_H
#include "../head/Gamestate.h"
#include <stdint.h>
#include <math.h>



Color GR0_IA_Random(GameState* state,Color player);

int max(int a, int b);

int min(int a, int b);
float exp_approx(float x);

Color GR0_minmax(GameState* state,Color player);

float GR0_alpha_beta_minmax(GameState* state, int depth,float alpha,float beta,Color player,int* best_move);

Color GR0_Glouton(GameState* state,Color player);

float heuristique(GameState* state,Color player);

float heuristique_mask(GameState* state,Color player,int x, int y);

float tanh_approx(float x);
#endif
