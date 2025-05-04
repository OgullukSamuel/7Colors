#ifndef CONSOLE_H
#define CONSOLE_H

#include "GR0_Gamestate.h"
#include "GR0_display.h"
#include "GR0_queue.h"
#include "GR0_map_functions.h"
#include "GR0_utilities.h"
#include "GR0_Agents.h"
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>




#define NUM_AGENT 10    //   /!\ Ne changer cette valeur que LE nombre d'agents dans l'array GR0_FUNC_ARRAY a aussi été modifié
#define NOMBRE_SIZE 13
#define NOMBRE_PARTIES 1400
#define NOMBRE_AVG 7
float GR0_Agent_vs_Agent(Color (*decision1)(GameState*,Color),Color (*decision2)(GameState*,Color),int affichage);

void GR0_elo_ranking(int choix,float* agents);



void GR0_plot_heuristique_mask(GameState* state);

void GR0_general_ranking();

void GR0_save_float_array_to_file(float array[NOMBRE_SIZE][NUM_AGENT], const char* filename);

int GR0_evaluation_main();

#endif