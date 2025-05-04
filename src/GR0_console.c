#include "../head/GR0_console.h"

int Size;

void GR0_save_float_array_to_file(float array[NOMBRE_SIZE][NUM_AGENT], const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    for (int i = 0; i < NOMBRE_SIZE; i++) {
        for (int j = 0; j < NUM_AGENT; j++) {
            fprintf(file, "%.6f", array[i][j]);
            if (j < NUM_AGENT - 1) {
                fputc(',', file);
            }
        }
        fputc('\n', file);
    }

    fclose(file);
}

float GR0_Agent_vs_Agent(Color (*decision1)(GameState*, Color), Color (*decision2)(GameState*, Color), int affichage) {
    // Fonction pour jouer une partie entre deux agents
    Queue moves[7];
    GR0_initQueues(moves);
    GameState etat={.map = NULL, .size = 0};
    GR0_initialize(&etat,Size);
	//GR0_plot_heuristique_mask(&etat);
    float fin = 0;
    int coup;

    while (fin == 0) {
        if (affichage) { GR0_plot(&etat); }
        GR0_resetQueues(moves);
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
        GR0_resetQueues(moves);
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
    GR0_freeQueues(moves);
    if (affichage) { GR0_plot(&etat); }

    fin = (fin ==3 ) ? 1.5 : fin;
    GR0_free_state(&etat);

    return fin;
}

void GR0_general_ranking(){
    int sizes[NOMBRE_SIZE];
    for(int i=0;i<NOMBRE_SIZE;i++){
        sizes[i]=i+3;
    }
    float agents[NOMBRE_SIZE][NUM_AGENT]={0};
    float agent_tempo[NOMBRE_AVG][NUM_AGENT]={0};
    clock_t start_time = clock();

    for (int i=0;i<NOMBRE_SIZE;i++){
        Size = sizes[i];
        printf("======================\nTaille de la carte : %d\n", Size);
        for(int j=0;j<NOMBRE_AVG;j++){
            
            printf("----------------------itération %d\n", j+1);
            GR0_elo_ranking(2,agent_tempo[j]);
            for (int k=0;k<NUM_AGENT;k++){
                agents[i][k]+=agent_tempo[j][k]/NOMBRE_AVG;
            }
        }
    }
    printf("Classement Elo des agents :\n");
    for (int i=0;i<NOMBRE_SIZE;i++){
        printf("Taille de la carte : %d\n", sizes[i]);
        for (int j=0;j<NUM_AGENT;j++){
            printf("Elo du joueur %d : %.2f\n", j+1, agents[i][j]);
        }
        printf("\n");
    }
    GR0_save_float_array_to_file(agents, "GR0_elo_ranking.csv");
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("temps d'execution de l'elo : %.6f seconds\n", elapsed_time);
}

void GR0_elo_ranking(int choix,float* agents) {
    // Fonction pour classer les agents selon leur classement Elo 
    func_ptr func_array[NUM_AGENT];

    if (choix == 1) {
        func_array[0] = &GR0_Glouton;
        func_array[1] = &GR0_minmax1;
        func_array[2] = &GR0_minmax2;
        func_array[3] = &GR0_minmax3;
        func_array[4] = &GR0_minmax4;
        func_array[5] = &GR0_minmax5;
        func_array[6] = &GR0_minmax6;
        func_array[7] = &GR0_minmax7;
        func_array[8] = &GR0_minmax8;
        func_array[9] = &GR0_minmax8;

    } else if(choix == 2) {
        func_array[0] = &GR0_IA_Random;
        func_array[1] = &GR0_Glouton;
        func_array[2] = &GR0_Glouton_heuristique;
        func_array[3] = &GR0_minmax3;
        func_array[4] = &GR0_minmax8;
        func_array[5] = &GR0_frontier_IA5;
        func_array[6] = &GR0_frontier_IA5_heuristique;
        func_array[7] = &GR0_frontier_IA8_heuristique;
        func_array[8] = &GR0_hegemonique;
        func_array[9] = &GR0_mixte;
    } else {
        //il faut mettre ici des foncteurs vers les autres agents que l'on souhaite tester
        func_array[0] = &GR0_IA_Random;
        func_array[1] = &GR0_Glouton;
        func_array[2] = &GR0_Glouton_heuristique;
        func_array[3] = &GR0_minmax3;
        func_array[4] = &GR0_minmax8;
        func_array[5] = &GR0_frontier_IA5;
        func_array[6] = &GR0_frontier_IA5_heuristique;
        func_array[7] = &GR0_hegemonique;
        func_array[8] = &GR0_mixte;
        func_array[9] = &GR0_minmax8;
    }


    printf("Début du classement Elo\n");
    float elos[NUM_AGENT];
    int matches[NUM_AGENT] = {0};
    const int nb_parties = NOMBRE_PARTIES;

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
        if(agents != NULL) {agents[i] = elos[i];}
    }

}

int GR0_evaluation_main(){
    // Fonction princpale pour le mode Arène
	IAS ia;
	GR0_question_IA_IA(&ia);
    
    if(ia.elo!=2){
        do{
            printf("Donne la taille de la carte que tu souhaites: ");
            scanf("%d", &Size);
        } while(Size<3 || Size>1000);
    }
    if(ia.elo==1){
        printf("choisis les agents:\n");
        printf("[1] Minmaxs   [2] Tous agents   [3] Agents externe (voir dans la fonction GR0_elo_ranking)\n");
        int choix = -1;
        do {
            printf("choix: ");
            scanf("%d", &choix);
        } while (choix != 1 && choix != 2 && choix != 3);
        clock_t start_time = clock();
        GR0_elo_ranking(choix,NULL);
        clock_t end_time = clock();
        double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        printf("temps d'execution de l'elo : %.6f seconds\n", elapsed_time);
    }
    else if(ia.elo==2){
        GR0_general_ranking();
    } else{
		double sum=0;
        float fin = 0;
		for(int i=0;i<ia.affrontements;i++){
            if(i%2){fin =2-GR0_Agent_vs_Agent(ia.decision1,ia.decision2,ia.affichage);
                sum+=fin;}
            else{fin =GR0_Agent_vs_Agent(ia.decision2,ia.decision1,ia.affichage)-1;
                sum+=fin;}
            if(fin==0.5){printf("Partie nulle !\n");}
            else if(fin==1){printf("Le joueur 1 a gagné !\n");}
            else if(fin==0){printf("Le joueur 2 a gagné !\n");}
            else{printf("Erreur dans le résultat de la partie %f!\n",fin);}
		}

		double winrate= ((double)sum/ia.affrontements)*100;
		printf("Winrate du joueur 1 :%.2f %% \n\n", winrate);	
	}
	return 0;
}
