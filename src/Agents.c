#include "../head/Agents.h"


Color GR0_IA_Random(GameState* state,Color player){
	Queue moves[7];
	GR0_get_move_available(state,player,moves);
	int i;
	do {
		i = GR0_get_random_scalar(0, 6);
	} while (moves[i].length == 0);
	Color move= i;
	return(move);
}

Color GR0_Glouton(GameState* state,Color player){
	Queue moves[7];
	uint8_t checkvar = GR0_get_move_available(state,player,moves);
	if (checkvar==0){
		printf("Le joueur %d ne peut pas jouer\n",player);
		return(-1);
	}
	int max=0;
	int index=0;
	int current=0;
	for(int i=0;i<7;i++){
		current = moves[i].length!=0 ? GR0_virtual_glouton_step(state, &moves[i],player) : current;
		if(current>max){
			max=current;
			index=i;
		}
	}
	Color indx = index;
	return(indx);
}




Color GR0_Agent_MinMax(GameState* state,Color player){
	//On explore en profondeur les coups possibles pour trouver ceux qui maximisent le nombre de cases explorees jusqu'à un degré n donné
	int profondeur = 3;
	Queue moves[7];
	GR0_get_move_available(state,player,moves);
	
}

float heuristique(GameState* state,Color player){
	//On calcule l'heuristique d'un état de jeu donné
	const int sizes = state->size;
	int pos[2]= { player == 1 ? 0 : state->size - 1,player == 1 ? state->size - 1 : 0 };
	Queue unexplored;
	Queue explored;
	initQueue(&unexplored);
	initQueue(&explored);
	GR0_get_network(state, pos, &explored, &explored);
	if (explored.length > (sizes * sizes) / 2) {
        return 1;
    }
	initQueue(&explored);
    initQueue(&unexplored);
    GR0_get_network(state, positio2, &explored, &explored);
    if (explored.length > (sizes * sizes) / 2) {
        return 2;
    } 
	
	
	
	float score = 0;
	
	while(explored.length!=0){
		int current[2];
		dequeue(&explored,current);
		score+=heuristique_mask(state,player,current[0],current[1]);
	}
	return(score);

}

float heuristique_mask(GameState* state,Color player,int x, int y){
	float size = state->size;
	float a=0.3;
	float delta=player==1? 1 : -1;
	return(1+delta*(tanh_approx(2*x/size)+tanh_approx(-2*y/size)+exp(-(x*x+y*y)/(size*size*a*a)))/1.52318831191);
}

float tanh_approx(float x){
	return(x*(27+x*x)/(27+9*x*x));
}