#ifndef GAME_STATE_H
#define GAME_STATE_H
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "/usr/include/python3.12/Python.h"
#include <math.h>
#include <stdint.h>
#include "../head/GR0_queue.h"
#include "../head/GR0_utilities.h"

typedef struct {
    PyObject_HEAD
	int* map;
	int size;
} PyscObject;


typedef struct GameState{
	int* map;
	int size;
} GameState;



void create_empty_game_state (PyscObject* state, int size);

void set_map_value (PyscObject* state, int x, int y, int value);

int get_map_value (PyscObject* state, int x, int y);

void fill_map(PyscObject* state);

void GR0_free_state(PyscObject* state);

void GR0_initialize(PyscObject* etat, int grid_size);

#endif