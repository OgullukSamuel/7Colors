#include "../head/map_functions.h"


void GR0_get_adjacent_cases(GameState* state, int x, int y, Queue* unexplored, Queue* explored, int SameColor, Queue* movements) {
    if(x>state->size || y>state->size || x<0 || y<0){
        printf("erreur dans les coordonnees dans adjactent cases\n");
        return;
    }

    Color color = get_map_value(state, x, y);
    int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (int i = 0; i < 4; i++) {
        int x_ = x + directions[i][0];
        int y_ = y + directions[i][1];

        if (x_ >= 0 && x_ < state->size && y_ >= 0 && y_ < state->size) {
            int pos[2]= {x_, y_};
            if (!GR0_isinQueue(explored, pos) && !GR0_isinQueue(unexplored, pos)) {
                Color c = get_map_value(state, x_, y_);
                if (c == color) {
                    GR0_enqueue(unexplored, pos);
                    
                } else if (!SameColor && c <= 9 && c >= 3) {
                    GR0_enqueue(movements, pos);
                }
            }
        }
    }

    int posi[2] = {x, y};
    GR0_enqueue(explored, posi);
    return;
}

void GR0_get_network(GameState* state, int* pos, Queue* explored, Queue* coup) {
    Queue unexplored;
    GR0_initQueue(&unexplored);
    GR0_enqueue(&unexplored, pos);
    
    while (unexplored.length != 0) {
        int next[2];
        GR0_dequeue(&unexplored, next);
        GR0_get_adjacent_cases(state, next[0], next[1], &unexplored, explored, 1, coup);
    }
    GR0_freeQueue(&unexplored);
    return;
}

void GR0_update_map(GameState* state, Queue* network, int player) {
    int next[2];
    while (network->length != 0) {
        GR0_dequeue(network, next);
        set_map_value(state, next[0], next[1], player);
    }
    return;
}

void GR0_step(GameState* state ,Queue* coup ,int player){
	Queue explored;
	GR0_initQueue(&explored);
	int current[2];
	while(coup->length!=0){
		GR0_dequeue(coup, current);
		GR0_get_network(state, current,  &explored,coup);
	}
	GR0_update_map(state, &explored, player);
    GR0_freeQueue(&explored);
    return;
}


int GR0_virtual_glouton_step(GameState* state ,Queue* coup ,int player){
	Queue explored;
	GR0_initQueue(&explored);
	int current[2];
	while(coup->length!=0){
		GR0_dequeue(coup, current);
		GR0_get_network(state, current,  &explored,coup);
	}
    int explolength = explored.length;
    GR0_freeQueue(&explored);
	return explolength ;
}


GameState GR0_virtual_depth_step(GameState* state ,Queue* coup ,int player){
	GameState new_state=GR0_copy_game_state(state);
	Queue explored;
	GR0_initQueue(&explored);
	int current[2];
	while(coup->length!=0){
		GR0_dequeue(coup, current);
		GR0_get_network(&new_state, current,  &explored,coup);
	}
	GR0_update_map(&new_state, &explored, player);
    GR0_freeQueue(&explored);
	return new_state;
}

GameState GR0_copy_game_state(GameState* original) {
    GameState copy;
    copy.size = original->size;
    copy.map = malloc(copy.size * copy.size * sizeof(Color));
    if (copy.map == NULL) {
        printf("ERMALLOC dans copy game state\n");
        exit(1);
    }
    for (int i = 0; i < copy.size * copy.size; i++) {
        copy.map[i] = original->map[i];
    }
    return copy;
}


uint8_t GR0_get_move_available(GameState* state,Color player,Queue moves[7]){
	int pos[2]= { player == 1 ? 0 : state->size - 1,player == 1 ? state->size - 1 : 0 };
	Queue unexplored;
    GR0_initQueue(&unexplored);
	Queue explored;
    GR0_initQueue(&explored);
    Queue movements;
    GR0_initQueue(&movements);
	GR0_enqueue(&unexplored, pos);
	int next[2];
	while (unexplored.length != 0) {
		GR0_dequeue(&unexplored, next);
		GR0_get_adjacent_cases(state, next[0], next[1], &unexplored, &explored, 0, &movements);
	}

	while (movements.length != 0) {
		GR0_dequeue(&movements, next);
		int color_index = get_map_value(state, next[0], next[1]) - 3;
        if (color_index >= 0 && color_index < 7) {
            GR0_enqueue(&moves[color_index], next);
	    }
    }

    GR0_freeQueue(&unexplored);
    GR0_freeQueue(&explored);
    GR0_freeQueue(&movements);
	
    return GR0_condenser(moves);
}

void GR0_get_total_moves(GameState* state, Color player, Queue* moves) {
    Queue moves2[7];
    GR0_initQueues(moves2);
    GR0_get_move_available(state, player, moves2);
    int pos[2];
    for (int i = 0; i < 7; i++) {
        while (moves2[i].length != 0) {
            GR0_dequeue(&moves2[i], pos);
            GR0_get_network(state, pos, &moves[i], NULL);
        }
    }
    GR0_freeQueues(moves2);
    return;
}

int GR0_partie_finie(GameState* state) {
    int size = state->size;
    int pos_j1[2] = {0, size - 1};
    int pos_j2[2] = {size - 1, 0};
    int length_j1, length_j2;

    Queue explored1;
    GR0_initQueue(&explored1);
    GR0_get_network(state, pos_j1, &explored1, NULL);
    length_j1 = explored1.length;
    GR0_freeQueue(&explored1);

    Queue explored2;
    GR0_initQueue(&explored2);
    GR0_get_network(state, pos_j2, &explored2, NULL);
    length_j2 = explored2.length;
    GR0_freeQueue(&explored2);

    if (length_j1 > (size * size) / 2 && length_j1 > length_j2) {
        return 1; // Joueur 1 gagne
    } else if (length_j2 > (size * size) / 2 && length_j2 > length_j1) {
        return 2; // Joueur 2 gagne
    } else if (length_j1 + length_j2 == size * size) {
        return 3; // Match nul
    }
    return 0; 
}