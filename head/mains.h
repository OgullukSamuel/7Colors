#ifndef MAINS_H
#define MAINS_H
#include "../head/Gamestate.h"
#include "../head/display.h"
#include "../head/queue.h"
#include "../head/map_functions.h"
#include "../head/utilities.h"
#include "../head/Agents.h"
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdint.h>



int main(int argc, char** argv);
int GR0_Agent_vs_Agent(Color (*decision1)(GameState*,Color),Color (*decision2)(GameState*,Color),int affichage);

void GR0_initialize(GameState* etat);

void GR0_elo_ranking();

void GR0_plot_heuristique_mask(GameState* state);
#endif