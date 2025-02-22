#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "../head/queue.h"
typedef enum Color{
	ERROR = -1,
	EMPTY,
	PLAYER_1,
	PLAYER_2,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	MAGENTA,
	CYAN,
	WHITE
}Color; 

typedef struct GameState{
	Color* map;
	int size;
} GameState;


void create_empty_game_state (GameState* state, int size);

void set_map_value (GameState* state, int x, int y, Color value);

Color get_map_value (GameState* state, int x, int y);

void fill_map(GameState* state);

int main(int argc, char** argv);

int GR0_get_random_scalar(int min, int max);

void GR0_free_state(GameState* state);

void GR0_get_network(GameState* state,int *pos , Queue* explored,Queue* coup);

void GR0_get_adjacent_cases(GameState* state, int x, int y, Queue* unexplored, Queue* explored, int SameColor, Queue* movements);

uint8_t GR0_get_move_available(GameState* state,Color player,Queue moves[7]);

uint8_t GR0_condenser(Queue* moves);

void GR0_decondenser(uint8_t condenser,int bits[7]);
void GR0_Agent_vs_Agent(Color (*decision1)(GameState*,Color),Color (*decision2)(GameState*,Color));

int GR0_virtual_glouton_step(GameState* state ,Queue* coup ,int player);

#endif