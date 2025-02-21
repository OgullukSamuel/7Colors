#include "../head/GameState.h"

#include "../head/display.h"
#include "../head/queue.h"


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
				if (SameColor==0) {
					if(get_map_value(state, x_, y_) != color){enqueue(movements, pos);}
                    
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
	int poulpe[2];
	displayQueue(coup);
	while(coup->length!=0){
		dequeue(coup, poulpe);
		printf("poulpe [%d,%d]\n",poulpe[0], poulpe[1]);
		GR0_get_network(state, poulpe,  &explored,coup);
		
	}
	
	GR0_update_map(state, &explored, player);
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



Color GR0_IA_Random(GameState* state,Color player){
	Queue moves[7];
	GR0_get_move_available(state,player,moves);
	int i;
	do {
		i = GR0_get_random_scalar(0, 6);
	} while (moves[i].length == 0);
	Color move= i+3;
	return(move);
}

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





void GR0_humain_vs_humain(){
	Queue territoire1;
	Queue territoire2;	
	Queue moves[7];
	GameState etat;
	initialize(&territoire1, &territoire2,&etat);
	int fin=0;
	int coup;
	uint8_t code_coups;
	while(fin==0){
		GR0_plot(&etat);
		code_coups= GR0_get_move_available(&etat, 1, moves);
		coup=GR0_get_user_input(1,code_coups);
		if (coup==-1){
			fin=2;
			break;
		}
		GR0_step(&etat, &moves[coup],1);
		GR0_plot(&etat);
		fin=GR0_partie_finie(&etat, &territoire1, &territoire2);
		if(fin!=0){break;}
		code_coups= GR0_get_move_available(&etat, 2, moves);
		coup=GR0_get_user_input(2,code_coups);
		if (coup==-1){
			fin=1;
			break;
		}

		GR0_step(&etat, &moves[coup],2);
		fin=GR0_partie_finie(&etat, &territoire1, &territoire2);
	}
	GR0_plot(&etat);
	if(fin==1){printf("Le joueur 1 a gagné\n");}
	else if(fin==2){printf("Le joueur 2 a gagné\n");}
	else{printf("Match nul\n");}
	GR0_free_state(&etat);
}

int main(int argc, char** argv){

	GR0_humain_vs_humain();
	
	return 0;
}

