#include "../head/GR0_main.h"

// Définition de la variable `state`
etat_jeu state = MENU;

int main(int argc, char** argv) {
    if (argc > 1) {
        if (argc >= 5 && strcmp(argv[2], "--ia") == 0) {
            int grid_size = atoi(argv[1]);
            const char* ia_name = argv[3];
            int player_color = atoi(argv[4]);

            if (grid_size < 2 || grid_size > 1000 || player_color < 1 || player_color > 2) {
                printf("mauvaise taille de grille ou couleur de joueurs.\n");
                return 1;
            }

            if (argc != 5 + grid_size * grid_size) {
                printf("nombre d'arguments invalides.\n");
                return 1;
            }

            GameState state;
            create_empty_game_state(&state, grid_size);
            for (int i = 0; i < grid_size * grid_size; i++) {
                state.map[i] = atoi(argv[5 + i]);
            }
            if(get_map_value(&state, state.size-1, 0) != 2 || get_map_value(&state, 0, state.size-1) != 1) {
                printf("Les case de départs des joueurs ne sont pas valides (ils doivent commencer dans les bons coins).\n");
                GR0_free_state(&state);
                return 1;
            }
            GR0_plot(&state);
            func_ptr ia_function = NULL;

            if (strcmp(ia_name, "random_player") == 0) {
                ia_function = &GR0_IA_Random;
            } else if (strcmp(ia_name, "glouton") == 0) {
                ia_function = &GR0_Glouton;
            } else if (strcmp(ia_name, "minmax3") == 0) {
                ia_function = &GR0_minmax3;
            } else if (strcmp(ia_name, "minmax8") == 0) {
                ia_function = &GR0_minmax8;
            } else if (strcmp(ia_name, "frontier_IA5") == 0) {
                ia_function = &GR0_frontier_IA5;
            } else if (strcmp(ia_name, "frontier_IA5_heuristique") == 0) {
                ia_function = &GR0_frontier_IA5_heuristique;
            } else if (strcmp(ia_name, "hegemonique") == 0) {
                ia_function = &GR0_hegemonique;
            } else if (strcmp(ia_name, "hegemonique_heuristique") == 0) {
                ia_function = &GR0_hegemonique_heuristique;
            } else if (strcmp(ia_name, "mixte") == 0) {
                ia_function = &GR0_mixte;
            } else if (strcmp(ia_name, "glouton_heuristique") == 0) {
                ia_function = &GR0_Glouton_heuristique;
            } else {
                printf("IA inconnue: %s\n", ia_name);
                GR0_free_state(&state);
                return 1;
            }

            if (ia_function) {
                Color move = ia_function(&state, player_color);
                printf("%d\n", move);
                GR0_free_state(&state);
                return 0;
            }
        } else {
            printf("Usage: %s taille_de_la_carte --ia nom_de_lia numero_du_joueur liste_etats\n", argv[0]);
            return 1;
        }
    }

    int choix = 0;
    printf("Bienvenue dans le jeu des 7 couleurs !\n");
    printf("Choisissez une option :\n");
    printf("[1] Jouer dans le mode Arène (pour l'évaluation et le test)\n");
    printf("[2] Jouer dans l'interface graphique\n");
    while (choix != 1 && choix != 2) {
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