#ifndef DISPLAY_H
#define DISPLAY_H
#include "../head/Gamestate.h"
#include "../head/Agents.h"
#include <stdio.h>

const char* GR0_get_color_code(Color c);

void GR0_plot(GameState* state) ;

const char* GR0_get_background_color_code(Color c) ;

int GR0_gameplay_question();

Color GR0_get_user_input(GameState* state,Color player);

void GR0_question_IA_IA(IAS* ia);
#endif