
#include "../head/GR0_display.h"
const char* GR0_get_color_code(Color c) {
    switch(c) {
        case ERROR: return "\033[31m";   // Rouge
        case EMPTY: return "\033[37m";   // Blanc
        case PLAYER_1: return "\033[34m"; // Bleu
        case PLAYER_2: return "\033[32m"; // Vert
        case RED: return "\033[31m";     // Rouge
        case GREEN: return "\033[32m";   // Vert
        case BLUE: return "\033[34m";    // Bleu
        case YELLOW: return "\033[33m";  // Jaune
        case MAGENTA: return "\033[35m"; // Magenta
        case CYAN: return "\033[36m";    // Cyan
        case WHITE: return "\033[37m";   // Blanc
        default: return "\033[0m";       // noir
    }
}

void GR0_plot(GameState* state) {
    // Fonction pour afficher l'état du jeu dans la console
    printf("┌");
    for (int j = 0; j < state->size; j++) {
        printf("───");
    }
    printf("┐\n");

    for (int i = 0; i < state->size; i++) {
        printf("│"); 
        for (int j = 0; j < state->size; j++) {
            Color c = state->map[state->size * i + j];
            const char* bg_color = GR0_get_background_color_code(c); 
            const char* text_color = GR0_get_color_code(c);          
			if(c==PLAYER_1){
				printf("%s%s 1 %s", bg_color, text_color, "\033[0m");	
			} else if (c==PLAYER_2) {
				printf("%s%s 2 %s", bg_color, text_color, "\033[0m");
			} else {
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

const char* GR0_get_background_color_code(Color c) {
    switch(c) {
        case ERROR: return "\033[48;5;1m";    // rouge
        case EMPTY: return "\033[48;5;15m";   // blanc
        case PLAYER_1: return "\033[48;5;4m"; // Bleu
        case PLAYER_2: return "\033[48;5;2m"; // Vert
        case RED: return "\033[48;5;9m";      // Rouge clair
        case GREEN: return "\033[48;5;10m";   // Vert clair
        case BLUE: return "\033[48;5;12m";    // Bleu clair
        case YELLOW: return "\033[48;5;11m";  // jaune
        case MAGENTA: return "\033[48;5;13m"; // Magenta
        case CYAN: return "\033[48;5;14m";    // Cyan
        case WHITE: return "\033[48;5;7m";    // Gris
        default: return "\033[48;5;15m";      // Default white
    }
}


Color GR0_get_user_input(GameState* state,Color player){
    // Fonction pour obtenir l'entrée de l'utilisateur
    Queue moves[7];
    GR0_initQueues(moves);
    uint8_t condenser=GR0_get_move_available(state, player, moves);
    if (condenser==0){
        //intf("Le joueur %d ne peut pas jouer\n",player);
        return(-1);
    }
    int disponibilite[7];
    for(int i=0;i<7;i++){
		disponibilite[i] = (moves[i].length > 0);
	}   
    GR0_freeQueues(moves);
    
    printf("Joueur %d, choisissez une couleur parmi les suivantes :\n", player);
    for(int i = 0; i < 7; i++) {
        if(disponibilite[i] == 1) {
            switch(i) {
                case 0:
                    printf("[R] Rouge   ");
                    break;
                case 1:
                    printf("[V] Vert   ");
                    break;
                case 2:
                    printf("[B] Bleu   ");
                    break;
                case 3:
                    printf("[J] Jaune   ");
                    break;
                case 4:
                    printf("[M] Magenta   ");
                    break;
                case 5:
                    printf("[C] Cyan   ");
                    break;
                case 6:
                    printf("[W] Blanc   ");
                    break;
            }
        }
    }
    printf("[Q] Quitter\n");  
    char choix;
    
    int indx=-2;
    while (indx==-2){
        scanf(" %c", &choix);
    
        switch (choix) {
            case 'R':
            case 'r':
                indx=disponibilite[0] ? 0 : -2;
                break;
            case 'V':	
            case 'v':
                indx=disponibilite[1] ? 1 : -2;
                break;
            case 'B':
            case 'b':
                indx=disponibilite[2] ? 2 : -2;
                break;
            case 'W':
            case 'w':
                indx= disponibilite[6] ? 6 : -2;
                break;
            case 'M':
            case 'm':
                indx= disponibilite[4] ? 4 : -2;
                break;
            case 'J':
            case 'j':
                indx=disponibilite[3] ? 3 : -2;
                break;
            case 'C':
            case 'c':
                indx=disponibilite[5] ? 5 : -2;
                break;
            case 'Q':
            case 'q':
                indx=-1;
                break;
        
    
            default:
                
                printf("\nTouche non reconnue : \n");
        }    
        if (indx==-2){printf("Le coup joué est invalide\n");}
    
    }
    

    return(indx);

}

void GR0_question_IA_IA(IAS* ia){
    // Fonction pour choisir les agents IA
	printf("Agent du joueur 1 : \n");
    printf("[0] Humain   [1] Random   [2] Glouton   [3] Glouton Heuristique   [4] MinMax3   [5] MinMax6   [6] Frontière5   [7] Frontière5+heuristique   [6] Mixte   [9] Hégémonique Heuristique [A] ELO classement   [B] classement général\n");    
    
    char ia1;
    int idx=0;
    while(idx==0){
        scanf("%c", &ia1);
        switch (ia1){
            case '0':
                idx=1;
                ia->decision1 = &GR0_get_user_input;    
                break;
            case '1':
                idx=1;
                ia->decision1 = &GR0_IA_Random;    
                break;
            case '2':
                idx=1;
                ia->decision1 = &GR0_Glouton;
                break;
            case '3':
                idx=1;
                ia->decision1 = &GR0_Glouton_heuristique;
                break;
            case '4':
                idx=1;
                ia->decision1 = &GR0_minmax3;
                break;
            case '5':
                idx=1;
                ia->decision1 = &GR0_minmax8;
                break;
            case '6':
                idx=1;
                ia->decision1 = &GR0_frontier_IA5;
                break;
            case '7':
                idx=1;
                ia->decision1 = &GR0_frontier_IA5_heuristique;
                break;
            
            case '8' :
                idx=1;
                ia->decision1 = &GR0_mixte;
                break;
            
            case '9':
                idx=1;
                ia->decision1 = &GR0_hegemonique_heuristique;
                break;
            case'A':
            case'a':
                idx=1;
                ia->decision1 = &GR0_IA_Random;
                ia->elo=1;
                return;
                break;
            case'B':
            case'b':
                idx=1;
                ia->decision1 = &GR0_IA_Random;
                ia->elo=2;
                return;
                break;

            default:
                printf("Choix invalide\n");
                break;
        }
    }
    printf("Agent du joueur 2 : \n");
    printf("[0] Humain   [1] Random   [2] Glouton   [3] Glouton Heuristique   [4] MinMax3   [5] MinMax6   [6] Frontière5   [7] Frontière5+heuristique   [8] Mixte   [9] Hégémonique Heuristique\n");    
    idx=0;
    int ia2;
    while(idx==0){
        scanf("%i", &ia2);
        switch (ia2){
            case 0:
                idx=1;
                ia->decision2 = &GR0_get_user_input;    
                break;
            
            case 1:
                idx=1;
                ia->decision2 = &GR0_IA_Random;    
                break;
            case 2:
                idx=1;
                ia->decision2 = &GR0_Glouton;
                break;
            case 3:
                idx=1;
                ia->decision2 = &GR0_Glouton_heuristique;
                break;
            case 4:
                idx=1;
                ia->decision2 = &GR0_minmax3;
                break;
            case 5:
                idx=1;
                ia->decision2 = &GR0_minmax6;
                break;

            case 6:
                idx=1;
                ia->decision2 = &GR0_frontier_IA5;
                break;
            case 7:
                idx=1;
                ia->decision2 = &GR0_frontier_IA5_heuristique;
                break;
            case 9:
                idx=1;
                ia->decision2 = &GR0_mixte;
                break;
            case 8:
                idx=1;
                ia->decision2 = &GR0_hegemonique_heuristique;
                break;
            
            default:
                printf("Choix invalide\n");
                break;
        }
    }
    printf("Combien d'affrontements souhaites tu voir: ");
	scanf("%i", &(ia->affrontements));
	printf("Souhaites-tu un affichage ? (1 pour oui, 0 pour non): ");
	scanf("%i", &(ia->affichage));
}