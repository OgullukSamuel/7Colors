#include "../head/sdl.h"

char* agent_names[NUM_AGENT] = {
    "Aléatoire(elo:1100)",
    "Glouton   (elo:1200)",
    "Glouton   Heuristique (elo 1300)",
    "Minmax3 (elo 1500)",
    "Minmax6 (elo 1800)",
    "Frontière5 ()",
    "Frontière5+heuristique (elo 1700)",
    "Hegemonique (elo 1500)",
    "Hegemonique + Heuristique"
};



func_ptr FUNC_ARRAY[NUM_AGENT] = {&GR0_IA_Random, &GR0_Glouton,&GR0_Glouton_heuristique, &GR0_minmax3, &GR0_minmax6, &GR0_frontier_IA5, &GR0_frontier_IA5_heuristique,&GR0_hegemonique, &GR0_hegemonique_heuristique};

SDL_Color colors[COLOR_COUNT + 1] = {
    {0, 0, 0, 255},         // 0 - Ne sera pas utilisé (éviter le 0)
    {255, 165, 0, 255},     // 1 - Joueur 1 Orange
    {128, 0, 128, 255},          // 1 - Joueur 2 Violet
    {255, 0, 0, 255},       // 3 - Rouge
    {0, 255, 0, 255},       // 4 - Vert
    {0, 0, 255, 255},       // 5 - Bleu
    {255, 255, 0, 255},     // 6 - Jaune
    {255, 0, 255, 255},     // 7 - Magenta
    {0, 255, 255, 255},     // 8 - Cyan
    {255, 255, 255, 255}    // 9 - Blanc
};

SDL_Texture* GR0_renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    if (!surface) {
        SDL_Log("Erreur TTF_RenderUTF8_Blended : %s", TTF_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        SDL_Log("Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
    }
    return texture;
}


void GR0_draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, int x, int y) {
    SDL_Texture* texture = GR0_renderText(renderer, font, text, color);
    if (!texture) return; // Ne rien faire si la texture est NULL

    int texW = 0, texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dst = {x - texW / 2, y - texH / 2, texW, texH};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

int GR0_in_rect(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

void GR0_update_cursor(GameState* etat,int current_player, int* cursor_position, int cursor_active) {
    if (cursor_active) {
        float evaluation= GR0_minmax8_evaluation(etat,current_player);
        printf("Evaluation : %f\n", evaluation);
        float maxreward=etat->size*etat->size;
        *cursor_position = GR0_clip(evaluation*100/maxreward,-100,100);
    }
}

void GR0_handle_grid_click(int mouseX, int mouseY, Queue* moves,int* current_player, GameState* etat, int agent, int* winner, int* cursor_position, int cursor_active,func_ptr FUNC_ARRAY[NUM_AGENT]) {

    int gy = (mouseX - GRID_OFFSET_X) / CELL_SIZE;
    int gx = (mouseY - GRID_OFFSET_Y) / CELL_SIZE;
    GR0_resetQueues(moves);
    GR0_get_move_available(etat, *current_player, moves);
    int coup_dispo = -1;
    for(int i=0;i<7;i++){
        if(GR0_isinQueue(&moves[i],(int[2]){gx,gy})){
            coup_dispo = i;
            printf("Case (%d,%d) disponible pour le joueur, couleur %d %d\n", gx, gy, *current_player, i);
            break;
        }
    }

    if (coup_dispo!=-1) {
        GR0_step(etat, &moves[coup_dispo], *current_player);
        GR0_update_cursor(etat,*current_player, cursor_position, cursor_active);
        int fin = GR0_partie_finie(etat);
        
        if (fin != 0) {
            if(fin!=3){
                printf("Partie terminée ! Joueur %d a gagné.\n", fin);
            }else{
                printf("Partie nulle !\n");
            }
            *winner = fin;
        }
        printf("agent %d\n", agent);
        *current_player= (*current_player==1)?2 :1;
        if(*current_player == 2 && agent!=-1){
            GR0_resetQueues(moves);
            GR0_get_move_available(etat, 2, moves);
            Color coup= FUNC_ARRAY[agent](etat, 2);
            if (coup == -1) {
                printf("L'adversaire abandonne\n");
            } else {
                printf("Coup de l'agent %d : (%d, %d)\n", agent, gx, gy);
                GR0_step(etat, &moves[coup], 2);
                GR0_update_cursor(etat,*current_player, cursor_position, cursor_active);
                int fin = GR0_partie_finie(etat);
                if (fin != 0) {
                    if(fin!=3){
                        printf("Partie terminée ! Joueur %d a gagné.\n", fin);
                    }else{
                        printf("Partie nulle !\n");
                    }
                    *winner = fin;
                }
                *current_player = 1;
            }
        } 
    }
}

void GR0_draw_cursor(SDL_Renderer* renderer, int position, int cursor_active, SDL_Color colors[COLOR_COUNT]) {
    if (!cursor_active) return;

    // Limites du curseur
    int cursor_min = -100;
    int cursor_max = 100;

    // Calcul de la position normalisée
    float norm = (float)(position - cursor_min) / (cursor_max - cursor_min);
    int cursor_y = GRID_OFFSET_Y + (int)(norm * (WINDOW_HEIGHT - GRID_OFFSET_Y * 2));

    // Décalage et largeur du curseur
    int cursor_x = 30; // Décalage vers la gauche
    int cursor_width = 20; // Largeur élargie
    SDL_Color c = colors[1];
    // Dessiner la zone bleue en haut
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255); // Bleu
    SDL_Rect blueZone = {cursor_x, GRID_OFFSET_Y, cursor_width, cursor_y - GRID_OFFSET_Y};
    SDL_RenderFillRect(renderer, &blueZone);
    c = colors[2];
    // Dessiner la zone rouge en bas
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255); // Rouge
    SDL_Rect redZone = {cursor_x, cursor_y, cursor_width, WINDOW_HEIGHT - cursor_y - GRID_OFFSET_Y};
    SDL_RenderFillRect(renderer, &redZone);

    // Dessiner le trait de séparation (curseur)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
    SDL_Rect line = {cursor_x, cursor_y - 1, cursor_width, 2}; // Ligne fine
    SDL_RenderFillRect(renderer, &line);

    // Dessiner une barre permanente au niveau de zéro
    int zero_y = GRID_OFFSET_Y + (WINDOW_HEIGHT - GRID_OFFSET_Y * 2) / 2; // Position verticale pour zéro
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Gris clair
    SDL_Rect zeroLine = {cursor_x - 10, zero_y - 1, cursor_width + 20, 2}; // Barre horizontale
    SDL_RenderFillRect(renderer, &zeroLine);

    // Afficher les valeurs 100 (en haut) et -100 (en bas)
    TTF_Font* font = TTF_OpenFont("arial.ttf", 16); // Charger une police (assurez-vous que la police est disponible)
    if (!font) {
        SDL_Log("Erreur chargement police : %s", TTF_GetError());
        return;
    }

    SDL_Color textColor = {255, 255, 255, 255}; // Blanc
    GR0_draw_text(renderer, font, "100", textColor, cursor_x + cursor_width / 2, GRID_OFFSET_Y - 10); // En haut
    GR0_draw_text(renderer, font, "-100", textColor, cursor_x + cursor_width / 2, WINDOW_HEIGHT - GRID_OFFSET_Y + 10); // En bas

    TTF_CloseFont(font); // Fermer la police
}
void GR0_draw_button(SDL_Renderer* renderer, SDL_Rect rect, const char* label, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}



void GR0_draw_slider(SDL_Renderer* renderer, int value) {
    // Ligne
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_Rect line = {SLIDER_X, SLIDER_Y, SLIDER_WIDTH, SLIDER_HEIGHT};
    SDL_RenderFillRect(renderer, &line);

    // Curseur
    float norm = (float)(value - GRID_MIN) / (GRID_MAX - GRID_MIN);
    int knobX = SLIDER_X + (int)(norm * SLIDER_WIDTH);

    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    SDL_Rect knob = {knobX - SLIDER_KNOB_RADIUS, SLIDER_Y - SLIDER_KNOB_RADIUS + SLIDER_HEIGHT/2,
                     SLIDER_KNOB_RADIUS * 2, SLIDER_KNOB_RADIUS * 2};
    SDL_RenderFillRect(renderer, &knob);
}



void GR0_draw_menu(SDL_Renderer* renderer, TTF_Font* font, int grid_size) {
    SDL_Rect startBtn = {300, 150, 200, 60};
    SDL_Rect opponentBtn = {300, 240, 200, 60};
    SDL_Rect quitBtn  = {300, 330, 200, 60};

    GR0_draw_button(renderer, startBtn, "", (SDL_Color){0, 200, 0, 255});
    GR0_draw_button(renderer, opponentBtn, "", (SDL_Color){0, 0, 200, 255});
    GR0_draw_button(renderer, quitBtn, "", (SDL_Color){200, 0, 0, 255});

    GR0_draw_text(renderer, font, "Démarrer", (SDL_Color){255,255,255,255}, startBtn.x + startBtn.w/2, startBtn.y + startBtn.h/2);
    GR0_draw_text(renderer, font, "Adversaire", (SDL_Color){255,255,255,255}, opponentBtn.x + opponentBtn.w/2, opponentBtn.y + opponentBtn.h/2);
    GR0_draw_text(renderer, font, "Quitter", (SDL_Color){255,255,255,255}, quitBtn.x + quitBtn.w/2, quitBtn.y + quitBtn.h/2);
    
    GR0_draw_text(renderer, font, "Taille de la grille :", (SDL_Color){255, 255, 255, 255}, WINDOW_WIDTH / 2, SLIDER_Y - 30);
    GR0_draw_slider(renderer, grid_size);

    char buf[20];
    snprintf(buf, sizeof(buf), "%d x %d", grid_size, grid_size);
    GR0_draw_text(renderer, font, buf, (SDL_Color){255, 255, 255, 255}, WINDOW_WIDTH / 2, SLIDER_Y + 30);
}

void GR0_draw_hovered_cell(SDL_Renderer* renderer, int mouseX, int mouseY, SDL_Color colors[COLOR_COUNT],GameState* etat) {
    int gy = (mouseX - GRID_OFFSET_X) / CELL_SIZE;
    int gx = (mouseY - GRID_OFFSET_Y) / CELL_SIZE;

    if (gx >= 0 && gx < etat->size && gy >= 0 && gy < etat->size) {
        SDL_Color c = colors[get_map_value(etat, gx, gy)];
        Queue hover_network;
        GR0_initQueue(&hover_network);

        GR0_get_network(etat, (int[2]){gx, gy}, &hover_network, NULL);
        int length = hover_network.length;

        int positions[length][2];
        for (int i = 0; i < length; i++) {
            GR0_dequeue(&hover_network, positions[i]);
        }

        #pragma omp parallel for
        for (int i = 0; i < length; i++) {
            SDL_Rect cell = {
                GRID_OFFSET_X + positions[i][1] * CELL_SIZE + 2,
                GRID_OFFSET_Y + positions[i][0] * CELL_SIZE + 2,
                CELL_SIZE - 4,
                CELL_SIZE - 4
            };
            #pragma omp critical
            {
                SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 50);
                SDL_RenderFillRect(renderer, &cell);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
                for (int j = 0; j < 2; j++) {
                    SDL_Rect border = {
                        cell.x - j, cell.y - j,
                        cell.w + 2 * j, cell.h + 2 * j
                    };
                    SDL_RenderDrawRect(renderer, &border);
                }
            }
        }

        GR0_freeQueue(&hover_network);

        // Afficher le numéro du joueur si la case appartient à un joueur
        int player = get_map_value(etat, gx, gy);
        if (player == PLAYER1_COLOR || player == PLAYER2_COLOR) {
            char playerText[2];
            snprintf(playerText, sizeof(playerText), "%d", player);
            GR0_draw_text(renderer, TTF_OpenFont("arial.ttf", 16), playerText,
                      (SDL_Color){255, 255, 255, 255},
                      GRID_OFFSET_X + gy * CELL_SIZE + CELL_SIZE / 2,
                      GRID_OFFSET_Y + gx * CELL_SIZE + CELL_SIZE / 2);
        }
    }
}

void GR0_draw_turn_info(SDL_Renderer* renderer, TTF_Font* font, int player) {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Tour du joueur %d", player);

    GR0_draw_text(renderer, font, buffer, (SDL_Color){255, 255, 255, 255},
              WINDOW_WIDTH / 2, GRID_OFFSET_Y - 20);
}

void GR0_draw_grid(SDL_Renderer* renderer, TTF_Font* font, GameState* etat, SDL_Color colors[COLOR_COUNT]) {
    if (etat->map == NULL) {
        SDL_Log("Erreur : la grille n'est pas initialisée.");
        return;
    }

    for (int y = 0; y < etat->size; y++) {
        for (int x = 0; x < etat->size; x++) {
            SDL_Color c = colors[get_map_value(etat,y,x)];
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

            SDL_Rect cell = {
                GRID_OFFSET_X + x * CELL_SIZE + CELL_PADDING / 2,
                GRID_OFFSET_Y + y * CELL_SIZE + CELL_PADDING / 2,
                CELL_SIZE - CELL_PADDING,
                CELL_SIZE - CELL_PADDING
            };
            SDL_RenderFillRect(renderer, &cell);

            // Afficher "1" ou "2" si c’est une cellule de joueur
            if (get_map_value(etat,y,x) == PLAYER1_COLOR) {
                GR0_draw_text(renderer, font, "1", (SDL_Color){0, 0, 0, 255},
                    cell.x + cell.w / 2, cell.y + cell.h / 2);
            } else if (get_map_value(etat,y,x) == PLAYER2_COLOR) {
                GR0_draw_text(renderer, font, "2", (SDL_Color){0, 0, 0, 255},
                    cell.x + cell.w / 2, cell.y + cell.h / 2);
            }
        }
    }
}
void GR0_draw_game_controls(SDL_Renderer* renderer, TTF_Font* font,int* cursor_active, int* winner) {
    // Décalage des boutons vers la droite
    int button_offset_x = GRID_OFFSET_X - 180; // Décalage pour éviter la collision avec la grille

    SDL_Rect cursorToggleBtn = {button_offset_x, GRID_OFFSET_Y, 150, 50}; // Bouton pour activer/désactiver le curseur
    SDL_Rect backToMenuBtn = {button_offset_x, GRID_OFFSET_Y + 60, 150, 50}; // Bouton pour revenir au menu

    // Définir la couleur du bouton en fonction de l'état du curseur
    SDL_Color button_color = *cursor_active ? (SDL_Color){0, 200, 0, 255} : (SDL_Color){200, 0, 0, 255};

    // Bouton pour activer/désactiver le curseur
    GR0_draw_button(renderer, cursorToggleBtn, "", button_color);

    // Dessiner le texte sur deux lignes
    GR0_draw_text(renderer, font, *cursor_active ? "Désactiver" : "Activer",
              (SDL_Color){0, 0, 0, 255}, cursorToggleBtn.x + cursorToggleBtn.w / 2, cursorToggleBtn.y + 10);
    GR0_draw_text(renderer, font, "Evaluation",
              (SDL_Color){0, 0, 0, 255}, cursorToggleBtn.x + cursorToggleBtn.w / 2, cursorToggleBtn.y + 30);

    // Bouton pour revenir au menu
    GR0_draw_button(renderer, backToMenuBtn, "", (SDL_Color){200, 200, 0, 255});
    GR0_draw_text(renderer, font, "Retour Menu",
              (SDL_Color){0, 0, 0, 255}, backToMenuBtn.x + backToMenuBtn.w / 2, backToMenuBtn.y + backToMenuBtn.h / 2);

    // Afficher le bouton de victoire si un joueur a gagné
    if (*winner != 0) {
        SDL_Rect replayBtn = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 30, 400, 60};
        GR0_draw_button(renderer, replayBtn, "", (SDL_Color){0, 200, 0, 255});
        char buffer[50];
        if(*winner !=3){snprintf(buffer, sizeof(buffer), "Le joueur %d a gagné ! Rejouer ?", *winner);
        }else{snprintf(buffer, sizeof(buffer), "Partie nulle ! Rejouer ?");}
        GR0_draw_text(renderer, font, buffer, (SDL_Color){255, 255, 255, 255}, replayBtn.x + replayBtn.w / 2, replayBtn.y + replayBtn.h / 2);
    }
}


int GR0_visual_main() {
    int current_player = 1;
    int winner = 0;
    int cursor_active = 0;
    int cursor_position = 0;
    int grid_size = 8;
    int agent =-1;
    GameState etat = {.map = NULL, .size = 0};

    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("7 Colors",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        SDL_Log("Erreur SDL_CreateWindow : %s", SDL_GetError());
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Erreur SDL_CreateRenderer : %s", SDL_GetError());
        SDL_DestroyWindow(window);
        return 1;
    }
    
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        SDL_Log("Erreur chargement police : %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 1;
    }
    etat_jeu state = MENU;
    GR0_initialize(&etat, grid_size);
    
    SDL_Event event;
    int running = 1;
    Queue moves[7];
    GR0_initQueues(moves);
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                    if (y >= SLIDER_Y && y <= SLIDER_Y + SLIDER_HEIGHT && state == MENU) {
                        int sliderX = x - SLIDER_X;
                        float norm = (float)sliderX / SLIDER_WIDTH;
                        int new_grid_size = (int)(GRID_MIN + norm * (GRID_MAX - GRID_MIN));
                    
                        if (new_grid_size < GRID_MIN) new_grid_size = GRID_MIN;
                        if (new_grid_size > GRID_MAX) new_grid_size = GRID_MAX;
                    
                        if (new_grid_size != grid_size) {
                            SDL_Log("Changement de taille de grille : %d -> %d", grid_size, new_grid_size);
                            grid_size = new_grid_size;
                            GR0_initialize(&etat, grid_size);
                        }
                    }
                
                    // Autres conditions pour les boutons (menu, quitter, etc.)
                if (state == MENU) {
                    SDL_Rect startBtn = {300, 150, 200, 60};
                    SDL_Rect opponentBtn = {300, 240, 200, 60};
                    SDL_Rect quitBtn = {300, 330, 200, 60};
                
                    if (GR0_in_rect(x, y, startBtn)) {
                        state = GAME;
                    } else if (GR0_in_rect(x, y, opponentBtn)) {
                        state = SELECT_OPPONENT;
                    } else if (GR0_in_rect(x, y, quitBtn)) {
                        running = 0;
                    }
                
                } else if (state == SELECT_OPPONENT) {
                    for (int i = 0; i < 8; ++i) {
                        SDL_Rect btn = {
                            100 + (i % 3) * 250, // Position horizontale (3 boutons par ligne)
                            100 + (i / 3) * 120, // Position verticale (3 lignes max)
                            200, // Largeur
                            80   // Hauteur
                        };
                        if (GR0_in_rect(x, y, btn)) {
                            printf("Agent : %s\n", agent_names[i]);
                            agent = i;
                            state = MENU;
                        }
                        GR0_draw_button(renderer, btn, "", (SDL_Color){0, 200, 200, 255});

                        // Afficher le nom de l'agent sur trois lignes
                        char line1[20] = {0}, line2[20] = {0}, line3[20] = {0};
                        snprintf(line1, sizeof(line1), "%.10s", agent_names[i]); // Première ligne (10 caractères max)
                        if (strlen(agent_names[i]) > 10) {
                            snprintf(line2, sizeof(line2), "%.10s", agent_names[i] + 10); // Deuxième ligne (10 caractères suivants)
                        }
                        if (strlen(agent_names[i]) > 20) {
                            snprintf(line3, sizeof(line3), "%.10s", agent_names[i] + 20); // Troisième ligne (reste du nom)
                        }

                        GR0_draw_text(renderer, font, line1, (SDL_Color){255, 255, 255, 255},
                                  btn.x + btn.w / 2, btn.y + btn.h / 4); // Ligne 1
                        if (strlen(line2) > 0) { // Afficher la deuxième ligne uniquement si elle n'est pas vide
                            GR0_draw_text(renderer, font, line2, (SDL_Color){255, 255, 255, 255},
                                      btn.x + btn.w / 2, btn.y + btn.h / 2); // Ligne 2
                        }
                        if (strlen(line3) > 0) { // Afficher la troisième ligne uniquement si elle n'est pas vide
                            GR0_draw_text(renderer, font, line3, (SDL_Color){255, 255, 255, 255},
                                      btn.x + btn.w / 2, btn.y + 3 * btn.h / 4); // Ligne 3
                        }
                    }
                } else if (state == GAME) {
                    GR0_handle_grid_click(x, y, moves, &current_player, &etat, agent, &winner, &cursor_position, cursor_active,FUNC_ARRAY);
                
                    // Gérer le bouton de victoire
                    if (winner != 0) {
                        SDL_Rect replayBtn = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 30, 300, 60};
                        if (GR0_in_rect(x, y, replayBtn)) {

                            GR0_initialize(&etat, grid_size);
                            current_player = 1;
                            winner = 0;
                            printf("Partie réinitialisée.\n");
                        }
                    }
                
                    SDL_Rect cursorToggleBtn = {50, GRID_OFFSET_Y, 150, 50};
                    SDL_Rect backToMenuBtn = {50, GRID_OFFSET_Y + 60, 150, 50};
                    if (GR0_in_rect(x, y, backToMenuBtn)) {
                        state = MENU; 
                        printf("Retour au menu principal.\n");
                    }
                    if (GR0_in_rect(x, y, cursorToggleBtn)) {
                        cursor_active = !cursor_active;
                        if(cursor_active) {
                            GR0_update_cursor(&etat,current_player, &cursor_position, cursor_active);
                        }
                        printf("Curseur %s\n", cursor_active ? "activé" : "désactivé");
                    }
                }
                    
                
            }
        }

        SDL_SetRenderDrawColor(renderer, 80, 45, 20, 255);
        SDL_RenderClear(renderer);

        if (state == MENU) {
            GR0_draw_menu(renderer, font, grid_size);
        } else if (state == GAME) {

            GR0_draw_turn_info(renderer, font, current_player);
            GR0_draw_grid(renderer, font, &etat, colors);
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            GR0_draw_hovered_cell(renderer, mx, my, colors, &etat);
            
            GR0_draw_cursor(renderer, cursor_position, cursor_active, colors);
            GR0_draw_game_controls(renderer, font, &cursor_active, &winner);

        } else if (state == SELECT_OPPONENT) {
            for (int i = 0; i < 8; ++i) {
                SDL_Rect btn = {
                    100 + (i % 3) * 250, // Position horizontale (3 boutons par ligne)
                    100 + (i / 3) * 120, // Position verticale (3 lignes max)
                    200, // Largeur
                    80   // Hauteur
                };
                GR0_draw_button(renderer, btn, "", (SDL_Color){0, 200, 200, 255});

                // Afficher le nom de l'agent sur trois lignes
                char line1[20] = {0}, line2[20] = {0}, line3[20] = {0};
                snprintf(line1, sizeof(line1), "%.10s", agent_names[i]); // Première ligne (10 caractères max)
                if (strlen(agent_names[i]) > 10) {
                    snprintf(line2, sizeof(line2), "%.10s", agent_names[i] + 10); // Deuxième ligne (10 caractères suivants)
                }
                if (strlen(agent_names[i]) > 20) {
                    snprintf(line3, sizeof(line3), "%.10s", agent_names[i] + 20); // Troisième ligne (reste du nom)
                }

                GR0_draw_text(renderer, font, line1, (SDL_Color){255, 255, 255, 255},
                          btn.x + btn.w / 2, btn.y + btn.h / 4); // Ligne 1
                if (strlen(line2) > 0) { // Afficher la deuxième ligne uniquement si elle n'est pas vide
                    GR0_draw_text(renderer, font, line2, (SDL_Color){255, 255, 255, 255},
                              btn.x + btn.w / 2, btn.y + btn.h / 2); // Ligne 2
                }
                if (strlen(line3) > 0) { // Afficher la troisième ligne uniquement si elle n'est pas vide
                    GR0_draw_text(renderer, font, line3, (SDL_Color){255, 255, 255, 255},
                              btn.x + btn.w / 2, btn.y + 3 * btn.h / 4); // Ligne 3
                }
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    TTF_CloseFont(font);
    font = NULL;
    SDL_Quit();
    TTF_Quit();
    GR0_freeQueues(moves);
    GR0_free_state(&etat);
    return 0;
}
