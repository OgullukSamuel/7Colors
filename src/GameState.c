#include "../head/GameState.h"

void create_empty_game_state (GameState* state, int size){
	state->size=size;
	state->map = malloc(size * size * sizeof(Color));
	if (state->map == NULL) {
    	printf("[ERREUR] MALLOC A ECHOUE!\n");
    	exit(1);
	}
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			state->map[i * size + j] = EMPTY;
		}
	}
}

void GR0_free_state(GameState* state){
	free(state->map);
}

void set_map_value (GameState* state, int x, int y, Color value){
	state->map[(state->size)*x+y] = value;
}


Color get_map_value (GameState* state, int x, int y){
    if (state->map == NULL || x >= state->size || y >= state->size || x < 0 || y < 0)
{
        printf("[ERREUR] map not big enough or not initialized %p %i access (%i %i)", state -> map, state->size, x, y);
        return ERROR;
    }
    return state->map[x * state->size + y];
}

void fill_map(GameState* map){
	for(int i=0;i<map->size*map->size;i++){
		map->map[i]=GR0_get_random_scalar(3,9);
	}
}

void GR0_initialize(GameState* etat, int grid_size) {
    if (etat->map != NULL) {
        GR0_free_state(etat); // Libérer la grille précédente si elle existe
    }
    srand(time(NULL) ^ clock());

	create_empty_game_state(etat,grid_size);
	fill_map(etat);

	set_map_value(etat, 0, grid_size-1, 1);
	set_map_value(etat, grid_size-1, 0, 2);
}