#include "../head/GR0_GameState.h"

PyObject* create_empty_game_state(PyscObject* state, int size){
	state->size=size;
	state->map = malloc(size * size * sizeof(int));
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			state->map[i * size + j] = 0;
		}
	}
	Py_RETURN_NONE;
}

void set_map_value (PyscObject* state, int x, int y, int value){
	state->map[(state->size)*x+y] = value;
}


int get_map_value(PyscObject* state, int x, int y){
    return state->map[x * state->size + y];
}

void fill_map(PyscObject* map){
	for(int i=0;i<map->size*map->size;i++){
		map->map[i]=GR0_get_random_scalar(3,9);
	}
}
