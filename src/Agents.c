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
	GR0_get_move_available(state,player,moves);
	int max=0;
	int index=0;
	int current;
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