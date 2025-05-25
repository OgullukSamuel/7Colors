#ifndef DISPLAY_H
#define DISPLAY_H
#include "../head/GR0_GameState.h"
#include "../head/GR0_Agents.h"
#include <stdio.h>

typedef struct IAS{
	int elo;
	int affrontements;
	int affichage;
	Color (*decision1)(GameState*,Color);
	Color (*decision2)(GameState*,Color);
} IAS;

const char* GR0_get_color_code(Color c);

void GR0_plot(GameState* state) ;

const char* GR0_get_background_color_code(Color c) ;

Color GR0_get_user_input(GameState* state,Color player);

void GR0_question_IA_IA(IAS* ia);
#endif