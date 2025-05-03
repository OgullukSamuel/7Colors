#include "../head/GR0_Agents.h"


Color GR0_IA_full_Random(GameState* state,Color player){
	// Fonction pour choisir un coup aléatoire
	return(rand() % 7);
}

Color GR0_IA_Random(GameState* state,Color player){
	// Fonction pour choisir un coup aléatoire parmis les coups possibles
	Queue moves[7];
	GR0_initQueues(moves);
	uint8_t mouvement = GR0_get_move_available(state,player,moves);
	GR0_freeQueues(moves);
	if(mouvement==0){
		return(-1);
	}
	int idx = GR0_random_bit_index(mouvement);
	Color coup = idx;
	return coup;
}

Color GR0_Glouton(GameState* state,Color player){
	// Algorithme glouton
	Queue moves[7];
	GR0_initQueues(moves);
	uint8_t checkvar = GR0_get_move_available(state,player,moves);
	if(checkvar==0){
		GR0_freeQueues(moves);
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
	GR0_freeQueues(moves);
	Color indx = index;
	return(indx);
}



Color GR0_Glouton_heuristique(GameState* state,Color player){
	// Algorithme glouton avec heuristique 
	int best_move;
	GR0_alpha_beta_minmax(state, 1, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&GR0_heuristique_minmax);  
	Color move = best_move;
	return move;
}

Color GR0_minmax1(GameState* state,Color player){
	// Algorithme MinMax avec profondeur 1
	int best_move;
	GR0_alpha_beta_minmax(state, 1, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&GR0_heuristique_minmax);  
	Color move = best_move;
	return move;
}

Color GR0_minmax2(GameState* state,Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 2, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&GR0_heuristique_minmax);  
	Color move = best_move;
	return move;
}

Color GR0_minmax3(GameState* state,Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 3, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&GR0_heuristique_minmax);  
	Color move = best_move;
	return move;
}

Color GR0_minmax4(GameState* state,Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 4, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&GR0_heuristique_minmax);  
	Color move = best_move;
	return move;
}

Color GR0_minmax5(GameState* state,Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 5, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&GR0_heuristique_minmax);  
	Color move = best_move;
	return move;
}

Color GR0_minmax6(GameState* state,Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 6, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&GR0_heuristique_minmax);  
	Color move = best_move;
	return move;
}

Color GR0_minmax7(GameState* state,Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 7, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&GR0_heuristique_minmax);  
	Color move = best_move;
	return move;
}

Color GR0_minmax8(GameState* state,Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 8, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&GR0_heuristique_minmax);  
	Color move = best_move;
	return move;
}

float GR0_minmax8_evaluation(GameState* state,Color player){
	int best_move;
	float eval=GR0_alpha_beta_minmax(state, 8, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&GR0_heuristique_minmax);
	return eval;
}

float GR0_alpha_beta_minmax(GameState* state, int depth, float alpha, float beta, Color player, int* best_move, float (*heuristique)(GameState*)) {
    if (GR0_partie_finie(state) || depth == 0) {
        return heuristique(state);
    }

    int tmp_best_move = -1;
    int stop_flag = 0;

    if (player == 1) {
        float max_eval = -10 * (state->size) * (state->size);
        Queue moves[7];
		GR0_initQueues(moves);
        GR0_get_move_available(state, player, moves);

        #pragma omp parallel for shared(alpha, beta, tmp_best_move, max_eval, stop_flag) schedule(dynamic)
        for (int i = 0; i < 7; i++) {
            if (stop_flag) continue;

            if (moves[i].length != 0) {
                GameState new_state = GR0_virtual_depth_step(state, &moves[i], player);
                int nulpts = -1;
                float eval = GR0_alpha_beta_minmax(&new_state, depth - 1, alpha, beta, 2, &nulpts, heuristique);
                GR0_free_state(&new_state);

                #pragma omp critical
                {
                    if (eval > max_eval) {
                        max_eval = eval;
                        tmp_best_move = i;
                    }
                    alpha = fmax(alpha, eval);
                    if (max_eval >= beta) {
                        stop_flag = 1;
                    }
                }
            }
        }
		GR0_freeQueues(moves);
        *best_move = tmp_best_move;
        return max_eval;
    } else {
        float min_eval = 10 * (state->size) * (state->size);
        Queue moves[7];
		GR0_initQueues(moves);
        GR0_get_move_available(state, player, moves);

        #pragma omp parallel for shared(alpha, beta, tmp_best_move, min_eval, stop_flag) schedule(dynamic)
        for (int i = 0; i < 7; i++) {
            if (stop_flag) continue;

            if (moves[i].length != 0) {
                GameState new_state = GR0_virtual_depth_step(state, &moves[i], player);
                int nulpts = -1;
                float eval = GR0_alpha_beta_minmax(&new_state, depth - 1, alpha, beta, 1, &nulpts, heuristique);
                GR0_free_state(&new_state);

                #pragma omp critical
                {
                    if (eval < min_eval) {
                        min_eval = eval;
                        tmp_best_move = i;
                    }
                    beta = fmin(beta, eval);
                    if (min_eval <= alpha) {
                        stop_flag = 1; // Set the flag to terminate other iterations
                    }
                }
            }
        }
		GR0_freeQueues(moves);
        *best_move = tmp_best_move;
        return min_eval;
    }
}
    
float GR0_heuristique_minmax(GameState* state){
	float maxreward=state->size*state->size*1.5;
    const int size = state->size;
    int pos_j1[2] = {0, size - 1};
    int pos_j2[2] = {size - 1, 0};
    Queue explored;
    
	float score = 0;
	GR0_initQueue(&explored);
    GR0_get_network(state, pos_j1 , &explored, &explored);
    int length_j1=explored.length;
	if (explored.length > (size * size) / 2) {
		GR0_freeQueue(&explored);
        return maxreward;
    }else {
		while(explored.length!=0){
			int current[2];
			GR0_dequeue(&explored,current);
			score+=GR0_heuristique_mask(state,current[0],current[1],1);
		}
	}
	

    GR0_resetQueue(&explored);
    GR0_get_network(state, pos_j2, &explored, &explored);
    if (explored.length > (size * size) / 2) {
		GR0_freeQueue(&explored);
        return -maxreward;
    }else if (length_j1 + explored.length == size * size) {
		GR0_freeQueue(&explored);
		return 0;
	}else{
		while(explored.length!=0){
			int current[2];
			GR0_dequeue(&explored,current);
			score-=GR0_heuristique_mask(state,current[0],current[1],2);
		}
	}
	GR0_freeQueue(&explored);
	return(score);
}

float GR0_heuristique_mask(GameState* state,int x, int y,Color player){
	float size = state->size;
	float halfsize =size/2;
	float a=0.3;
	float x_=x-halfsize, y_=y-halfsize;
	float delta= (player==1 ? 1 : -1);
	return (1+delta*(GR0_tanh_approx(2*x_/size)+GR0_tanh_approx(-2*y_/size)+delta*GR0_exp_approx(-(x_*x_+y_*y_)/(size*size*a*a)))*1.5);
	//return(1)   mettre ça si on veut tester l'algorithme minmax sans heuristique
}

float GR0_heuristique_frontier(GameState* state){
	// Fonction pour calculer l'heuristique de la frontière5 ( c'est une sorte de variante de l'algorithme hegemonique rendue globale)
	Queue moves[7];
	GR0_initQueues(moves);
	GR0_get_move_available(state, 1, moves);
	float frontier=0;
	for(int i=0;i<7;i++){
		frontier+=moves[i].length;
	}
	GR0_freeQueues(moves);
	Queue moves2[7];
	GR0_initQueues(moves2);
	GR0_get_move_available(state, 2, moves2);
	for(int i=0;i<7;i++){
		frontier-=moves2[i].length;
	}
	GR0_freeQueues(moves2);
	return(frontier);
}

float GR0_heuristique_frontier_upgraded(GameState* state){ 
	// Fonction pour calculer l'heuristique de frontière5 avec le masque 
	Queue moves[7], moves2[7];
	GR0_initQueues(moves);
	GR0_initQueues(moves2);
	GR0_get_move_available(state, 1, moves);
	GR0_get_move_available(state, 2, moves2);

	float frontier = 0;

	for (int i = 0; i < 7; i++) {
		while (moves[i].length != 0) {
			int current[2];
			GR0_dequeue(&moves[i], current);
			frontier += GR0_heuristique_mask(state, current[0], current[1],1);
		}
		while (moves2[i].length != 0) {
			int current[2];
			GR0_dequeue(&moves2[i], current);
			frontier -= GR0_heuristique_mask(state, current[0], current[1],2);
		}
	}
	GR0_freeQueues(moves);
	GR0_freeQueues(moves2);
	return frontier;
}


Color GR0_frontier_IA5_heuristique(GameState* state, Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 5, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&GR0_heuristique_frontier_upgraded);  
	Color move = best_move;
	return move;
}
Color GR0_frontier_IA5(GameState* state, Color player){
	int best_move;
	GR0_alpha_beta_minmax(state, 5, -10 * (state->size) * (state->size), 10 * (state->size) * (state->size), player, &best_move,&GR0_heuristique_frontier);  
	Color move = best_move;
	return move;
}

Color GR0_mixte(GameState* state, Color player){
	Queue moves[7];
	GR0_initQueues(moves);
	uint8_t checkvar = GR0_get_move_available(state,player,moves);
	if(checkvar==0){
		GR0_freeQueues(moves);
		return(-1);
	}

	int cases_atteignables=0;
	for(int i=0;i<7;i++){
		cases_atteignables+=moves[i].length;
	}
	int max=0;
	int index=0;
	int current=0;
	for(int i=0;i<7;i++){
		current=0;
		GR0_resetQueues(moves);
		GameState new_state = GR0_virtual_depth_step(state, &moves[i], player);
		GR0_get_move_available(&new_state, player, moves);
		for(int i=0;i<7;i++){
			current+=moves[i].length;
		}
		if(current>max){
			max=current;
			index=i;
		}
	}
	GR0_freeQueues(moves);
	if(max>cases_atteignables){
		Color indx = index;
		return(indx);		
	}else {
		return(GR0_Glouton(state,player));
	}
}

Color GR0_hegemonique(GameState* state, Color player){
	Queue moves[7];
	GR0_initQueues(moves);
	uint8_t checkvar = GR0_get_move_available(state,player,moves);
	if(checkvar==0){
		GR0_freeQueues(moves);
		return(-1);
	}
	int max=0;
	int argmax=0;
	for(int i=0;i<7;i++){
		if(moves[i].length>max){
			max=moves[i].length;
			argmax=i;
		}
	}
	GR0_freeQueues(moves);
	
	return(argmax);
}


Color GR0_hegemonique_heuristique(GameState* state, Color player){
	Queue moves[7];
	GR0_initQueues(moves);
	uint8_t checkvar = GR0_get_move_available(state,player,moves);
	if(checkvar==0){
		GR0_freeQueues(moves);
		return(-1);
	}

	int cases_atteignables=0;
	for(int i=0;i<7;i++){
		for(int j=0;j<moves[i].length;j++){
			int current[2];
			GR0_dequeue(&moves[i],current);
			cases_atteignables+=GR0_heuristique_mask(state,current[0],current[1],player);
		}
	}
	int max=0;
	int index=0;
	int current=0;
	for(int i=0;i<7;i++){
		current=0;
		GameState new_state = GR0_virtual_depth_step(state, &moves[i], player);
		GR0_resetQueues(moves);	
		GR0_get_move_available(&new_state, player, moves);
		for(int i=0;i<7;i++){
			for(int j=0;j<moves[i].length;j++){
				int current_pos[2];
				GR0_dequeue(&moves[i],current_pos);
				current+=GR0_heuristique_mask(state,current_pos[0],current_pos[1],player);
			}
		} 
		if(current>max){
			max=current;
			index=i;
		}
	}
	GR0_freeQueues(moves);
	if(max>cases_atteignables){
		Color indx = index;
		return(indx);		
	}else {
		return(GR0_Glouton_heuristique(state,player));
	}
}