#include "../head/Agents.h"
#include "../head/utilities.h"
#include <stdio.h>
#include <math.h> // Include math.h for fmax and fmin

Color GR0_IA_Random(GameState* state,Color player){
	Queue moves[7];
	uint8_t mouvement = GR0_get_move_available(state,player,moves);
	int idx = GR0_random_bit_index(mouvement);
	Color coup = idx;
	return coup;
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



Color GR0_minmax3(GameState* state,Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 3, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&heuristique_minmax);  
	Color move = best_move;
	//printf("Joueur : %i Evaluation : %f et coup joué %d \n", player,eval, best_move+3);
	
	return move;
}
Color GR0_minmax6(GameState* state,Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 6, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&heuristique_minmax);  
	Color move = best_move;
	//printf("Joueur : %i Evaluation : %f et coup joué %d \n", player,eval, best_move+3);
	
	return move;
}


float GR0_alpha_beta_minmax(GameState* state, int depth,float alpha,float beta,Color player,int* best_move,float (*heuristique)(GameState*)){
	if (GR0_partie_finie(state) || depth == 0) {
		return heuristique(state);
	}
	int nulpts=-1;
	int tmp_best_move = -1;
	if (player == 1) {
		float max_eval = -10 * (state->size) * (state->size);
		Queue moves[7];
		GR0_get_move_available(state, player, moves);
		for (int i = 0; i < 7; i++) {
			if (moves[i].length != 0) {
				GameState new_state = GR0_virtual_depth_step(state, &moves[i], player, depth);
				float eval = GR0_alpha_beta_minmax(&new_state, depth - 1, alpha, beta, 2,&nulpts,heuristique);
				GR0_free_state(&new_state);
				if (eval > max_eval){
					max_eval = eval;
					tmp_best_move = i;
				}
				alpha = fmax(alpha, eval);
				if (max_eval >= alpha) {
					break;
				}
			}
		}
		*best_move = tmp_best_move;
		return max_eval;
	} else {
		float min_eval = 10 * (state->size) * (state->size);
		Queue moves[7];
		GR0_get_move_available(state, player, moves);
		for (int i = 0; i < 7; i++) {
			if (moves[i].length != 0) {
				GameState new_state = GR0_virtual_depth_step(state, &moves[i], player, depth);
				int nullptr=-1;
				float eval = GR0_alpha_beta_minmax(&new_state, depth - 1, alpha, beta, 1,&nullptr,heuristique);
				GR0_free_state(&new_state);
				if (eval < min_eval){
					min_eval = eval;
					tmp_best_move = i;
				}
				beta = fmin(beta, eval);
				if (min_eval <= alpha) {
					break;
				}
			}
		}
		*best_move = tmp_best_move;
		return min_eval;
	}
}
    


float heuristique_minmax(GameState* state){
	float maxreward=1001;
    const int size = state->size;
    int pos_j1[2] = {0, size - 1};
    int pos_j2[2] = {size - 1, 0};
    Queue explored;
    
	float score = 0;
	initQueue(&explored);
    GR0_get_network(state, pos_j1 , &explored, &explored);
    int length_j1=explored.length;
	if (explored.length > (size * size) / 2) {
        return maxreward;
    }else {
		while(explored.length!=0){
			int current[2];
			dequeue(&explored,current);
			score+=heuristique_mask(state,current[0],current[1]);
		}
	}
	

    initQueue(&explored);
    GR0_get_network(state, pos_j2, &explored, &explored);
    if (explored.length > (size * size) / 2) {
        return -maxreward;
    }else if (length_j1 + explored.length == size * size) {
		return 0;
	}else{
		while(explored.length!=0){
			int current[2];
			dequeue(&explored,current);
			score-=heuristique_mask(state,current[0],current[1]);
		}
	}
	return(score);

}

float heuristique_mask(GameState* state,int x, int y){
	
	float size = state->size;
	float halfsize =size/2;
	float a=0.3;
	float x_=x-halfsize, y_=y-halfsize;
	return (1+(tanh_approx(2*x_/size)+tanh_approx(-2*y_/size)+exp_approx(-(x_*x_+y_*y_)/(size*size*a*a)))*0.6565);
}

float heuristique_frontier(GameState* state){
	Queue moves[7];
	GR0_get_move_available(state, 1, moves);
	float frontier=0;
	for(int i=0;i<7;i++){
		frontier+=moves[i].length;
	}
	Queue moves2[7];
	GR0_get_move_available(state, 2, moves2);
	for(int i=0;i<7;i++){
		frontier-=moves[i].length;
	}
	return(frontier);
}

float heuristique_frontier_upgraded(GameState* state) {
	Queue moves[7], moves2[7];
	GR0_get_move_available(state, 1, moves);
	GR0_get_move_available(state, 2, moves2);

	float frontier = 0;

	for (int i = 0; i < 7; i++) {
		while (moves[i].length != 0) {
			int current[2];
			dequeue(&moves[i], current);
			frontier += heuristique_mask(state, current[0], current[1]);
		}
		while (moves2[i].length != 0) {
			int current[2];
			dequeue(&moves2[i], current);
			frontier -= heuristique_mask(state, current[0], current[1]);
		}
	}

	return frontier;
}


Color GR0_frontier_IA1(GameState* state, Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 1, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&heuristique_frontier);  
	Color move = best_move;
	
	return move;
}

Color GR0_frontier_IA5(GameState* state, Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 5, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&heuristique_frontier);  
	Color move = best_move;
	
	return move;
}


Color GR0_frontier_IA5_heuristique(GameState* state, Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 5, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&heuristique_frontier);  
	Color move = best_move;
	
	return move;
}