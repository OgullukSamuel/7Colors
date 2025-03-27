
#include "../head/Gamestate.h"
#include <stdio.h>
#include "../head/display.h"
const char* get_color_code(Color c) {
    switch(c) {
        case ERROR: return "\033[31m";   // Red for ERROR
        case EMPTY: return "\033[37m";   // White for EMPTY
        case PLAYER_1: return "\033[34m"; // Blue for PLAYER_1
        case PLAYER_2: return "\033[32m"; // Green for PLAYER_2
        case RED: return "\033[31m";     // Red for RED
        case GREEN: return "\033[32m";   // Green for GREEN
        case BLUE: return "\033[34m";    // Blue for BLUE
        case YELLOW: return "\033[33m";  // Yellow for YELLOW
        case MAGENTA: return "\033[35m"; // Magenta for MAGENTA
        case CYAN: return "\033[36m";    // Cyan for CYAN
        case WHITE: return "\033[37m";   // White for WHITE
        default: return "\033[0m";       // Default (reset color)
    }
}

void GR0_plot(GameState* state) {

    printf("┌");
    for (int j = 0; j < state->size; j++) {
        printf("───");
    }
    printf("┐\n");

    for (int i = 0; i < state->size; i++) {
        printf("│"); 
        for (int j = 0; j < state->size; j++) {
            Color c = state->map[state->size * i + j];
            const char* bg_color = get_background_color_code(c); 
            const char* text_color = get_color_code(c);          
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

const char* get_background_color_code(Color c) {
    switch(c) {
        case ERROR: return "\033[48;5;1m";    // Red background for ERROR
        case EMPTY: return "\033[48;5;15m";   // White background for EMPTY
        case PLAYER_1: return "\033[48;5;4m"; // Blue background for PLAYER_1
        case PLAYER_2: return "\033[48;5;2m"; // Green background for PLAYER_2
        case RED: return "\033[48;5;9m";      // Bright Red background for RED
        case GREEN: return "\033[48;5;10m";   // Bright Green background for GREEN
        case BLUE: return "\033[48;5;12m";    // Bright Blue background for BLUE
        case YELLOW: return "\033[48;5;11m";  // Yellow background for YELLOW
        case MAGENTA: return "\033[48;5;13m"; // Magenta background for MAGENTA
        case CYAN: return "\033[48;5;14m";    // Cyan background for CYAN
        case WHITE: return "\033[48;5;7m";    // Gray background for WHITE
        default: return "\033[48;5;15m";      // Default white background
    }
}
/*
int GR0_gameplay_question(){
    printf("Selectionnez votre mode de jeu :\n");
    printf("[1] Humain vs Humain\n");
    printf("[2] Humain vs IA\n");
    printf("[3] IA vs IA\n");
    printf("[4] Quitter\n");
    int choix;
    scanf("%d", &choix);
    return choix;
}
*/



Color GR0_get_user_input(GameState* state,Color player){
    Queue moves[7];
    uint8_t condenser=GR0_get_move_available(state, player, moves);
    if (condenser==0){
        printf("Le joueur %d ne peut pas jouer\n",player);
        return(-1);
    }
    int disponibilite[7];
    for(int i=0;i<7;i++){
		disponibilite[i] = (moves[i].length > 0);
	}
    
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

	printf("Agent du joueur 1 : \n");
    printf("[1] Humain   [2] Random   [3] Glouton   [4] MinMax3   [5] MinMax6   [6] Frontiere1   [7] Frontière5   [8] Frontière5+ heuristique\n");    
    
    int ia1;
    int idx=0;
    while(idx==0){
        scanf("%i", &ia1);
        switch (ia1){
            case 1:
                idx=1;
                ia->decision1 = &GR0_get_user_input;    
                break;
            case 2:
                idx=1;
                ia->decision1 = &GR0_IA_Random;    
                break;
            case 3:
                idx=1;
                ia->decision1 = &GR0_Glouton;
                break;
            case 4:
                idx=1;
                ia->decision1 = &GR0_minmax3;
                break;
            case 5:
                idx=1;
                ia->decision1 = &GR0_minmax6;
                break;
            case 6:
                idx=1;
                ia->decision1 = &GR0_frontier_IA1;
                break;

            case 7:
                idx=1;
                ia->decision1 = &GR0_frontier_IA5;
                break;
            case 8:
                idx=1;
                ia->decision1 = &GR0_frontier_IA5_heuristique;
                break;
            
            default:
                printf("Choix invalide\n");
                break;
        }
    }
    printf("Agent du joueur 2 : \n");
    printf("[1] Humain   [2] Random   [3] Glouton   [4] MinMax3   [5] MinMax6   [6] Frontiere1   [7] Frontiere5   [8] Frontière5+ heuristique   \n");    
    idx=0;
    int ia2;
    while(idx==0){
        scanf("%i", &ia2);
        switch (ia2){
            case 1:
                idx=1;
                ia->decision2 = &GR0_get_user_input;    
                break;
            
            case 2:
                idx=1;
                ia->decision2 = &GR0_IA_Random;    
                break;
            case 3:
                idx=1;
                ia->decision2 = &GR0_Glouton;
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
                ia->decision2 = &GR0_frontier_IA1;
                break;

            case 7:
                idx=1;
                ia->decision2 = &GR0_frontier_IA5;
                break;
            case 8:
                idx=1;
                ia->decision2 = &GR0_frontier_IA5_heuristique;
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