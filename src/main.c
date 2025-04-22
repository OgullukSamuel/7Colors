#include "../head/main.h"

// Définition de la variable `state`
etat_jeu state = MENU;


int main(int argc, char** argv){
    int choix = 0;
    printf("Bienvenue dans le jeu des 7 couleurs !\n");
    printf("Choisissez une option :\n");
    printf("[1] Jouer dans la console (pour l'évaluation et le test)\n");
    printf("[2] Jouer dans l'interface graphique\n");
    while(choix != 1 && choix != 2) {
        printf("Entrez votre choix (1 ou 2) : ");
        scanf("%d", &choix);
        if (choix == 1) {
            GR0_evaluation_main();
            break;
        } else if (choix == 2) {
            GR0_visual_main();
            break;
        } else {
            printf("Choix invalide. Veuillez choisir 1 ou 2.\n");
        }
    }
    return 0;
    
}