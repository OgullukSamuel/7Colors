#include "../head/map_functions.h"


void GR0_get_adjacent_cases(GameState* state, int x, int y, Queue* unexplored, Queue* explored, int SameColor, Queue* movements) {
    Color color = get_map_value(state, x, y);
    int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (int i = 0; i < 4; i++) {
        int x_ = x + directions[i][0];
        int y_ = y + directions[i][1];

        if (x_ >= 0 && x_ < state->size && y_ >= 0 && y_ < state->size) {
            int pos[2] = {x_, y_};
            if (!isinQueue(explored, pos) && !isinQueue(unexplored, pos)) {
                Color c = get_map_value(state, x_, y_);
                if (SameColor && c == color) {
                    enqueue(unexplored, pos);
                } else if (!SameColor && c != color && c <= 9 && c >= 3) {
                    enqueue(movements, pos);
                }
            }
        }
    }

    int posi[2] = {x, y};
    enqueue(explored, posi);
}

void GR0_get_network(GameState* state, int* pos, Queue* explored, Queue* coup) {
    Queue unexplored;
    initQueue(&unexplored);

    enqueue(&unexplored, pos);

    while (unexplored.length != 0) {
        int next[2];
        dequeue(&unexplored, next);
        GR0_get_adjacent_cases(state, next[0], next[1], &unexplored, explored, 1, coup);
    }
}

void GR0_update_map(GameState* state, Queue* network, int player) {
    int next[2];
    while (network->length != 0) {
        dequeue(network, next);
        if (get_map_value(state, next[0], next[1]) != player) {
            set_map_value(state, next[0], next[1], player);
        }
    }
}

void GR0_step(GameState* state ,Queue* coup ,int player){
	Queue explored;
	initQueue(&explored);
	int current[2];
	while(coup->length!=0){
		dequeue(coup, current);
		GR0_get_network(state, current,  &explored,coup);
		
	}
	
	GR0_update_map(state, &explored, player);
}


int GR0_virtual_glouton_step(GameState* state ,Queue* coup ,int player){
	Queue explored;
	initQueue(&explored);
	int current[2];
	while(coup->length!=0){
		dequeue(coup, current);
		GR0_get_network(state, current,  &explored,coup);
	}
	return(explored.length);
}


GameState GR0_virtual_depth_step(GameState* state ,Queue* coup ,int player){
	GameState new_state=GR0_copy_game_state(state);
	Queue explored;
	initQueue(&explored);
	int current[2];
	while(coup->length!=0){
		dequeue(coup, current);
		GR0_get_network(&new_state, current,  &explored,coup);
	}
	GR0_update_map(&new_state, &explored, player);
	return(new_state);
}

GameState GR0_copy_game_state(GameState* original) {
    GameState copy;
    copy.size = original->size;
    copy.map = (Color*)malloc(copy.size * copy.size * sizeof(Color));
    if (copy.map == NULL) {
        printf("=Memory allocation failed!\n");
        exit(1);
    }
    for (int i = 0; i < copy.size * copy.size; i++) {
        copy.map[i] = original->map[i];
    }
    return copy;
}


uint8_t GR0_get_move_available(GameState* state,Color player,Queue moves[7]){
	for(int i=0;i<7;i++){
		initQueue(&moves[i]);
	}
	int pos[2]= { player == 1 ? 0 : state->size - 1,player == 1 ? state->size - 1 : 0 };
	Queue unexplored;
    initQueue(&unexplored);
	Queue explored;
    initQueue(&explored);
	Queue movements;
	initQueue(&movements);
	enqueue(&unexplored, pos);
	int next[2];
	while(unexplored.length!=0){
		dequeue(&unexplored,next);
		GR0_get_adjacent_cases(state, next[0],next[1], &unexplored, &explored,0,&movements);
	}

	while(movements.length!=0){
		//displayQueue(&movements);
		dequeue(&movements,next);
		enqueue(&moves[get_map_value(state,next[0] , next[1])-3], next);
	}
	return(GR0_condenser(moves));
}



int GR0_partie_finie(GameState* state) {
    int size = state->size;
    int pos_j1[2] = {0, size - 1};
    int pos_j2[2] = {size - 1, 0};
    int length_j1, length_j2;

    Queue explored1;
    initQueue(&explored1);
    GR0_get_network(state, pos_j1, &explored1, NULL);
    length_j1 = explored1.length;

    Queue explored2;
    initQueue(&explored2);
    GR0_get_network(state, pos_j2, &explored2, NULL);

    length_j2 = explored2.length;

    if (length_j1 > (size * size) / 2 && length_j1 > length_j2) {
        return 1; // Joueur 1 gagne
    }
    if (length_j2 > (size * size) / 2 && length_j2 > length_j1) {
        return 2; // Joueur 2 gagne
    }
    if (length_j1 + length_j2 == size * size) {
        return 3; // Match nul
    }
    return 0; 
}