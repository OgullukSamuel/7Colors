#include "../head/console.h"

int Size;

int GR0_Agent_vs_Agent(Color (*decision1)(GameState*, Color), Color (*decision2)(GameState*, Color), int affichage) {
    Queue moves[7];
    initQueues(moves);
    GameState etat={.map = NULL, .size = 0};
    GR0_initialize(&etat,Size);
	//GR0_plot_heuristique_mask(&etat);
    float fin = 0;
    int coup;

    while (fin == 0) {
        if (affichage) { GR0_plot(&etat); }
        resetQueues(moves);
        // Joueur 1
        uint8_t coup_dispo1=GR0_get_move_available(&etat, 1, moves);
        if (coup_dispo1 == 0) {
            fin = 2; // Joueur 2 gagne si Joueur 1 ne peut pas jouer
            break;
        }
        coup = decision1(&etat, 1);
        if (coup == -1) {
            fin = 2; 
            break;
        }
        GR0_step(&etat, &moves[coup], 1);

        if (affichage) { GR0_plot(&etat); }
        fin = GR0_partie_finie(&etat);
        if (fin != 0) { break; }
        resetQueues(moves);
        uint8_t coup_dispo2=GR0_get_move_available(&etat, 2, moves);
        if (coup_dispo2 == 0) {
            fin = 1; // Joueur 1 gagne si Joueur 2 ne peut pas jouer
            break;
        }
        coup = decision2(&etat, 2);
        if (coup == -1) {
            fin = 1; 
            break;
        }
        GR0_step(&etat, &moves[coup], 2);

        fin = GR0_partie_finie(&etat);
    }
    freeQueues(moves);
    if (affichage) { GR0_plot(&etat); }

    fin = (fin ==3 ) ? 1.5 : fin;
    GR0_free_state(&etat);

    return fin;
}

void GR0_elo_ranking() {
    
    func_ptr func_array[NUM_AGENT] = {&GR0_IA_Random, &GR0_Glouton,&GR0_Glouton_heuristique, &GR0_minmax3, &GR0_minmax6, &GR0_frontier_IA5, &GR0_frontier_IA5_heuristique,&GR0_hegemonique, &GR0_hegemonique_heuristique};
    float elos[NUM_AGENT];
    int matches[NUM_AGENT] = {0};
    const int nb_parties = 10000;

    for (int i = 0; i < NUM_AGENT; i++) {
        elos[i] = 1500;
    }

    #pragma omp parallel for shared(elos, matches) schedule(dynamic)
    for (int i = 0; i < nb_parties; i++) {
        if (i % 1000 == 0) {
            printf("Partie %d sur %d\n", i, nb_parties);
        }
        int j1 = GR0_get_random_scalar(0, NUM_AGENT - 1);
        int j2;
        do {
            j2 = GR0_get_random_scalar(0, NUM_AGENT - 1);
        } while (j1 == j2);

        float elo_j1, elo_j2, D, prob_j1, prob_j2, match_result, K1, K2;

        #pragma omp critical
        {
            elo_j1 = elos[j1];
            elo_j2 = elos[j2];
        }

        D = elo_j1 - elo_j2;
        prob_j1 = 1 / (1 + pow(10, -D / 400));
        prob_j2 = 1 - prob_j1;

        match_result = GR0_Agent_vs_Agent(func_array[j1], func_array[j2], 0);
        match_result = (match_result == 1.5) ? 0.5 : (match_result == 2 ? 0 : 1);

        K1 = (matches[j1] < 30) ? 40 : (elos[j1] < 2400 ? 20 : 10);
        K2 = (matches[j2] < 30) ? 40 : (elos[j2] < 2400 ? 20 : 10);

        #pragma omp critical
        {
            elos[j1] += K1 * (match_result - prob_j1);
            elos[j2] += K2 * ((1 - match_result) - prob_j2);
            matches[j1]++;
            matches[j2]++;
        }
    }

    for (int i = 0; i < NUM_AGENT; i++) {
        printf("Elo du joueur %d : %.2f (matches joués : %d)\n", i + 1, elos[i], matches[i]);
    }
}

void time_GR0_elo_ranking() {
    GR0_elo_ranking();
}

int evaluation_main(){
	IAS ia;
	GR0_question_IA_IA(&ia);
    do{printf("Donne la taille de la carte que tu souhaites (entre 3 et 100): ");
        scanf("%d", &Size);
    }while(Size<3 || Size>100);
	
    if(ia.elo==1){
		time_function("GR0_elo_ranking", time_GR0_elo_ranking);
	} else{
		double sum=0;
		for(int i=0;i<ia.affrontements;i++){
            
            float fin =(float)(2-GR0_Agent_vs_Agent(ia.decision1,ia.decision2,ia.affichage));
			sum+=fin;
            if (fin == 1) { printf("Le joueur 1 a gagné\n"); 
            }else if (fin == 0) { printf("Le joueur 2 a gagné\n"); 
            } else { printf("Match nul\n"); fin = 3. / 2; }

		}
		double winrate= ((double)sum/ia.affrontements)*100;
		printf("Winrate du joueur 1 :%.2f %% \n\n", winrate);	
	}

	
	return 0;
}
