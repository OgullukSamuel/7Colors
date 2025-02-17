#include "../head/GameState.h"
#include "../head/queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

GameState state = {.map = NULL, .size = 0};

void create_empty_game_state (GameState* state, int size){
	state->size=size;
	state->map = (Color*)malloc(size * size * sizeof(Color));
	if (state->map == NULL) {
    	printf("[ERROR] Memory allocation failed!\n");
    	exit(1);
	}
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			state->map[i * size + j] = EMPTY;  // Correctly initialize all cells in the map.
		}
	}
}

void GR0_free_state(GameState* state){
	free(state->map);
}

int GR0_get_random_scalar(int min, int max){
	int value= min + rand()%(max-min+1);
	return(value);
}

void set_map_value (GameState* state, int x, int y, Color value){
	state->map[(state->size)*x+y] = value;
}


Color get_map_value (GameState* state, int x, int y){
	if (state->map == NULL || x >= state->size || y >= state->size || x < 0 || y < 0)
	{
		printf("[ERROR] map not big enough or not initialized %p %i access (%i %i)", state -> map, state -> size, x, y);
		return ERROR;
	}
	return state -> map[x * (state -> size) + y];
}

void fill_map(GameState* map){
	for(int i=0;i<(map->size)*(map->size);i++){
		map->map[i]=GR0_get_random_scalar(3,9);
	}
}



const char* get_color_code(Color c) {
    switch(c) {
        case ERROR: return "\033[31m";   // Red for ERROR
        case EMPTY: return "\033[37m";   // White for EMPTY
        case PLAYER_1: return "\033[34m"; // Blue for PLAYER_1
        case PLAYER_2: return "\033[32m"; // Green for PLAYER_2
        case RED: return "\033[31m";     // Red for RED
        case GREEN: return "\033[32m";   // Green for GREEN
        case BLUE: return "\033[34m";    // Blue for BLUE
        case YELLOW: return "\033[33m";  // Yellow for YELLOW
        case MAGENTA: return "\033[35m"; // Magenta for MAGENTA
        case CYAN: return "\033[36m";    // Cyan for CYAN
        case WHITE: return "\033[37m";   // White for WHITE
        default: return "\033[0m";       // Default (reset color)
    }
}

void GR0_plot(GameState* state) {
    printf("┌");
    for (int j = 0; j < state->size; j++) {
        printf("───");
    }
    printf("┐\n");

    for (int i = 0; i < state->size; i++) {
        printf("│"); 
        for (int j = 0; j < state->size; j++) {
            Color c = state->map[state->size * i + j];
            const char* bg_color = get_background_color_code(c); 
            const char* text_color = get_color_code(c);          
			if(c==PLAYER_1){
				printf("%s%s 1 %s", bg_color, text_color, "\033[0m");	
			}
			else if (c==PLAYER_2) {
				printf("%s%s 2 %s", bg_color, text_color, "\033[0m");
			}
            else {
				printf("%s%s O %s", bg_color, text_color, "\033[0m");
			}
        }
        printf("│\n");

        if (i < state->size - 1) {
            printf("├");
            for (int j = 0; j < state->size; j++) {
                printf("───");
            }
            printf("┤\n");
        }
    }

    printf("└");
    for (int j = 0; j < state->size; j++) {
        printf("───");
    }
    printf("┘\n");

    printf("\033[0m");
}

const char* get_background_color_code(Color c) {
    switch(c) {
        case ERROR: return "\033[48;5;1m";    // Red background for ERROR
        case EMPTY: return "\033[48;5;15m";   // White background for EMPTY
        case PLAYER_1: return "\033[48;5;4m"; // Blue background for PLAYER_1
        case PLAYER_2: return "\033[48;5;2m"; // Green background for PLAYER_2
        case RED: return "\033[48;5;9m";      // Bright Red background for RED
        case GREEN: return "\033[48;5;10m";   // Bright Green background for GREEN
        case BLUE: return "\033[48;5;12m";    // Bright Blue background for BLUE
        case YELLOW: return "\033[48;5;11m";  // Yellow background for YELLOW
        case MAGENTA: return "\033[48;5;13m"; // Magenta background for MAGENTA
        case CYAN: return "\033[48;5;14m";    // Cyan background for CYAN
        case WHITE: return "\033[48;5;7m";    // Gray background for WHITE
        default: return "\033[48;5;15m";      // Default white background
    }
}

void GR0_get_adjacent_cases(GameState* state, int x, int y, Queue* unexplored, Queue* explored, int SameColor, Queue* movements) {
    int size = state->size;
    Color color = get_map_value(state, x, y);
    int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (int i = 0; i < 4; i++) {
        int x_ = x + directions[i][0];
        int y_ = y + directions[i][1];

        if (x_ >= 0 && x_ < size && y_ >= 0 && y_ < size) {
            int pos[2] = {x_, y_};
            if (!isinQueue(explored, pos) && !isinQueue(unexplored, pos)) {
                if (get_map_value(state, x_, y_) == color) {
                    enqueue(unexplored, pos);
                }
				if (SameColor==0 && get_map_value(state, x_, y_) != color) {
                    enqueue(movements, pos);
                }
            }
        }
    }

    int posi[2] = {x, y};
    enqueue(explored, posi);
}

void GR0_get_network(GameState* state, int x, int y, Queue* explored) {
    Queue unexplored;
    initQueue(&unexplored);
    initQueue(explored);

    int pos[2] = {x, y};
    enqueue(&unexplored, pos);
    
    while (unexplored.length != 0) {
        int next[2];
        dequeue(&unexplored, next);
        GR0_get_adjacent_cases(state, next[0], next[1], &unexplored, explored, 1, &unexplored);
    }
}


void GR0_update_map(GameState* state,Queue* network,int player){
	// ici on vient update les couleurs de la map un réseau de meme couleurs et un joueur
	int size=state->size;
	int next[2];
	while(network->length!=0){
		dequeue(network, next);
		set_map_value(state, next[0],next[1], player);
	}
}

void GR0_step(GameState* state ,int x, int y ,int player){
	set_map_value(state, x, y, player);
	int size=state->size;
	Queue explored;
	GR0_get_network(state, x, y,  &explored);
	GR0_update_map(state, &explored, player);

}



void GR0_get_move_available(GameState* state,int player,Queue moves[7]){
	for(int i=0;i<7;i++){
		initQueue(&moves[i]);
	}
	int pos[2]= { player == 1 ? state->size - 1 : 0,player == 1 ? 0 : state->size - 1 };
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
}
/*
Color GR0_IA_Random(GameState* state,Color player){
	GR0_get_move_available();
	//Color move=;
	return(move);
}
*/
int GR0_partie_finie(GameState* state,Queue* territoire1,Queue* territoire2){
	//0 si la partie n'est pas finie,1 si 1 a gagné , 2 si 2 a gagné
	if(territoire1->length>state->size/2){return(1);}
	else if(territoire2->length>state->size/2){return(2);}
	else {return(0);}
}

void initialize(Queue* territoire1,Queue* territoire2,GameState* etat){
	int size;
    srand(time(NULL));
    printf("Donne la taille de la carte que tu souhaites: ");
    scanf("%d", &size);
    
    // Print the entered number
    printf("La t: %d\n", size);
	initQueue(territoire1);
	int position1[2]={0,size-1};
	int position2[2]={size-1,0};
	enqueue(territoire1, position1);
	
	initQueue(territoire2);
	enqueue(territoire2, position2);

	create_empty_game_state(etat,size);
	fill_map(etat);

	set_map_value(etat, 0, size-1, 1);
	set_map_value(etat, size-1, 0, 2);

}


int main(int argc, char** argv){
	Queue territoire1;
	Queue territoire2;	
	Queue moves[7];
	GameState etat;
    initialize(&territoire1, &territoire2,&etat);


	GR0_plot(&etat);
	//GR0_plot(&etat);
	GR0_get_move_available(&etat, 1, moves);
	for(int i=0;i<7;i++){
		displayQueue(&moves[i]);
	}
	GR0_step(&etat, 1, 5, 1);
	GR0_plot(&etat);
	
	
	GR0_free_state(&etat);
}

