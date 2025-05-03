#ifndef CONSOLE_H
#define CONSOLE_H

#include "Gamestate.h"
#include "display.h"
#include "queue.h"
#include "map_functions.h"
#include "utilities.h"
#include "Agents.h"
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdint.h>




#define NUM_AGENT 9
float GR0_Agent_vs_Agent(Color (*decision1)(GameState*,Color),Color (*decision2)(GameState*,Color),int affichage);

void GR0_elo_ranking(int choix,float* agents);



void GR0_plot_heuristique_mask(GameState* state);

void save_float_array_to_file(float array[13][9], const char* filename);

int GR0_evaluation_main();

#endif