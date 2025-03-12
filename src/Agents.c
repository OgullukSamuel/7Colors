#include "../head/Agents.h"
#include <stdio.h>


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

int max(int a, int b){return a > b ? a : b;}


int min(int a, int b){return a > b ? b : a;}

Color GR0_minmax(GameState* state,Color player){
	int best_move;
	float eval = GR0_alpha_beta_minmax(state, 3, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move);
	Color move = best_move+3;
	printf("Evaluation : %f", eval);
	return move;
}	

float GR0_alpha_beta_minmax(GameState* state, int depth,float alpha,float beta,Color player,int* best_move){
	if (GR0_partie_finie(state) || depth == 0) {
		return heuristique(state, player);
	}
	int tmp_best_move = -1;
	if (player == 1) {
		float max_eval = 10 * (state->size) * (state->size);
		Queue moves[7];
		GR0_get_move_available(state, player, moves);
		for (int i = 0; i < 7; i++) {
			if (moves[i].length != 0) {
				GameState new_state = GR0_virtual_depth_step(state, &moves[i], player, depth);
				float eval = GR0_alpha_beta_minmax(&new_state, depth - 1, alpha, beta, 2,NULL);
				GR0_free_state(&new_state);
				if (eval > max_eval){
					max_eval = eval;
					tmp_best_move = i;
				}
				alpha = max(alpha, eval);
				if (max_eval >= alpha) {
					break;
				}
			}
		}
		*best_move = tmp_best_move;
		return max_eval;
	} else {
		float min_eval = -10 * (state->size) * (state->size);
		Queue moves[7];
		GR0_get_move_available(state, player, moves);
		for (int i = 0; i < 7; i++) {
			if (moves[i].length != 0) {
				GameState new_state = GR0_virtual_depth_step(state, &moves[i], player, depth);
				float eval = GR0_alpha_beta_minmax(&new_state, depth - 1, alpha, beta, 1,NULL);
				GR0_free_state(&new_state);
				if (eval < min_eval){
					min_eval = eval;
					tmp_best_move = i;
				}
				beta = min(beta, eval);
				if (min_eval <= alpha) {
					break;
				}
			}
		}
		*best_move = tmp_best_move;
		return min_eval;
	}
}
    


float heuristique(GameState* state,Color player){
	float maxreward=1000;
    const int size = state->size;
    int pos_j1[2] = {0, size - 1};
    int pos_j2[2] = {size - 1, 0};
    int length_j1;
    Queue explored;
    
    initQueue(&explored);
    GR0_get_network(state, player == 1 ?pos_j2 :pos_j1 , &explored, &explored);
    length_j1 = explored.length;
    if (length_j1 > (size * size) / 2) {
        return -maxreward;
    }
    
    initQueue(&explored);
    GR0_get_network(state, player == 1 ?pos_j1 :pos_j2, &explored, &explored);
    if (explored.length > (size * size) / 2) {
        return maxreward;
    }
	if (length_j1 + explored.length == size * size) {
        return 0;
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
	float halfsize =size/2;
	float a=0.3;
	float x_=x-halfsize, y_=y-halfsize;
	float delta=player==1? 1 : -1;
	return delta*(1+delta*(tanh_approx(2*x_/size)+tanh_approx(-2*y_/size)+exp_approx(-(x_*x_+y_*y_)/(size*size*a*a)))*0.656517643);
}

float exp_approx(float x){
	return(1+x*(1+x/2*(1+x/3*(1+x/4))));
}

float tanh_approx(float x){
	return(x*(27+x*x)/(27+9*x*x));
}