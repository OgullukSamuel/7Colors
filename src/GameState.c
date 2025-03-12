#include "../head/GameState.h"
#include "../head/Agents.h"
#include "../head/display.h"
#include "../head/queue.h"
#include <stdio.h>


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
	printf("Filling map\n");
	for(int i=0;i<(map->size)*(map->size);i++){
		map->map[i]=GR0_get_random_scalar(3,9);
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
                Color c = get_map_value(state, x_, y_);
				if (c == color) {
                    enqueue(unexplored, pos);
                }
				if (SameColor==0) {
					if(c != color && c<=9 && c>=3){enqueue(movements, pos);}
                    
                }
            }
        }
    }

    int posi[2] = {x, y};
    enqueue(explored, posi);
}

void GR0_get_network(GameState* state,int *pos , Queue* explored,Queue* coup){ 
    Queue unexplored;
    initQueue(&unexplored);

    enqueue(&unexplored, pos);
    
    while (unexplored.length != 0) {
        int next[2];
        dequeue(&unexplored, next);
        GR0_get_adjacent_cases(state, next[0], next[1], &unexplored, explored, 1, &unexplored);
    }
}



void GR0_update_map(GameState* state,Queue* network,int player){
	// ici on vient update les couleurs de la map un réseau de meme couleurs et un joueur
	int next[2];
	while(network->length!=0){
		dequeue(network, next);
		set_map_value(state, next[0],next[1], player);

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


GameState GR0_virtual_depth_step(GameState* state ,Queue* coup ,int player,int depth){
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
        printf("[ERROR] Memory allocation failed!\n");
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
    // 0 : partie non finie, 1 : joueur 1 gagne, 2 : joueur 2 gagne, 3 : match nul
    const int size = state->size;
    int pos_j1[2] = {0, size - 1};
    int pos_j2[2] = {size - 1, 0};
    int length_j1;
    Queue explored;
    
    initQueue(&explored);
    GR0_get_network(state, pos_j1, &explored, &explored);
    length_j1 = explored.length;
    if (length_j1 > (size * size) / 2) {return 1;}
    
    initQueue(&explored);
    GR0_get_network(state, pos_j2, &explored, &explored);
    if (explored.length > (size * size) / 2) {return 2;}
    
    if (length_j1 + explored.length == size * size) {return 3;}
    
    return 0;
}

void initialize(GameState* etat){
	int size;
    srand(time(NULL));
    printf("Donne la taille de la carte que tu souhaites: ");
    scanf("%d", &size);
    
    printf("La t: %d\n", size);

	create_empty_game_state(etat,size);
	fill_map(etat);

	set_map_value(etat, 0, size-1, 1);
	set_map_value(etat, size-1, 0, 2);

}

uint8_t GR0_condenser(Queue* moves){
	uint8_t result = 0;
	for(int i=0;i<7;i++){
		result |= (moves[i].length > 0) << i;
	}
	return(result);
}

void GR0_decondenser(uint8_t condenser,int bits[7]){
	for (int i = 0; i < 7; i++) {
		bits[i] = (condenser >> i) & 1;
	}
}	





void GR0_Agent_vs_Agent(Color (*decision1)(GameState*,Color),Color (*decision2)(GameState*,Color)){
	Queue moves[7];
	GameState etat;
	initialize(&etat);
	int fin=0;
	int coup;
	while(fin==0){
		GR0_plot(&etat);
		GR0_get_move_available(&etat, 1, moves);
		coup = decision1(&etat,1);
		printf("coup1 : %d\n",coup);
		if (coup==-1){
			fin=2;
			break;
		}
		GR0_step(&etat, &moves[coup],1);
		GR0_plot(&etat);
		fin=GR0_partie_finie(&etat);
		if(fin!=0){break;}
		GR0_get_move_available(&etat, 2, moves);
		coup = decision2(&etat,2);
		printf("coup2 : %d\n",coup);
		if (coup==-1){
			fin=1;
			break;
		}

		GR0_step(&etat, &moves[coup],2);
		fin=GR0_partie_finie(&etat);
	}
	GR0_plot(&etat);
	if(fin==1){printf("Le joueur 1 a gagné\n");}
	else if(fin==2){printf("Le joueur 2 a gagné\n");}
	else{printf("Match nul\n");}
	GR0_free_state(&etat);
}


int main(int argc, char** argv){
	int game_mode = GR0_gameplay_question();
	switch (game_mode) {
		case 1:
			GR0_Agent_vs_Agent(&GR0_get_user_input,&GR0_get_user_input);
			break;
		case 2:
			GR0_Agent_vs_Agent(&GR0_get_user_input,&GR0_minmax);
			break;
		case 3:
			GR0_Agent_vs_Agent(&GR0_Glouton,&GR0_Glouton);
			break;
		case 4:
			return 0;
		default:
			printf("Mode de jeu inconnu !\n");
			break;
	}
	return 0;
}

//l'algo glouton pete un plomb parfois et décide juste de all in sur rouge meme si il peut meme pas jouer rouge ( c'est indépendant du numéro du joueur)