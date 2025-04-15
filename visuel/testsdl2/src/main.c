
#include "../head/main.h"




int current_player = 1;

typedef enum { MENU, GAME, SELECT_OPPONENT } etat_jeu;

func_ptr func_array[NUM_AGENT] = {&GR0_IA_Random, &GR0_Glouton,&GR0_Glouton_heuristique, &GR0_minmax3, &GR0_minmax6, &GR0_frontier_IA5, &GR0_frontier_IA5_heuristique,&GR0_hegemonique, &GR0_hegemonique_heuristique};

char* agent_names[NUM_AGENT] = {
    "Aléatoire",
    "Glouton",
    "Glouton_heuristique",
    "Minmax3",
    "Minmax6",
    "Frontier_IA5",
    "Frontier_IA5_heuristique",
    "Hegemonique",
    "Hegemonique_heuristique"
};

int grid_size = 8;
int agent =-1;

GameState etat = {.map = NULL, .size = 0};

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color) {
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

void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, int x, int y) {
    SDL_Texture* texture = renderText(renderer, font, text, color);
    if (!texture) return; // Ne rien faire si la texture est NULL

    int texW = 0, texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dst = {x - texW / 2, y - texH / 2, texW, texH};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}
SDL_Color colors[COLOR_COUNT + 1] = {
    {0, 0, 0, 255},         // 0 - Ne sera pas utilisé (éviter le 0)
    {255, 50, 50, 255},     // 1 - Joueur 1
    {50, 50, 255, 255},     // 2 - Joueur 2
    {255, 0, 0, 255},       // 3 - Rouge
    {0, 255, 0, 255},       // 4 - Vert
    {0, 0, 255, 255},       // 5 - Bleu
    {255, 255, 0, 255},     // 6 - Jaune
    {255, 0, 255, 255},     // 7 - Magenta
    {0, 255, 255, 255},     // 8 - Cyan
    {255, 255, 255, 255}    // 9 - Blanc
};



/*
int GR0_Agent_vs_Agent(Color (*decision1)(GameState*, Color), Color (*decision2)(GameState*, Color), int affichage) {
    Queue moves[7];
    initQueues(moves);
    GameState etat;
    GR0_initialize(&etat);
	//GR0_plot_heuristique_mask(&etat);
    float fin = 0;
    int coup;

    while (fin == 0) {
        GR0_plot(&etat);
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
*/

void GR0_initialize(){
    srand(time(NULL) ^ clock());

	create_empty_game_state(&etat,grid_size);
	fill_map(&etat);

	set_map_value(&etat, 0, grid_size-1, 1);
	set_map_value(&etat, grid_size-1, 0, 2);
}

void init_grid() {
    if (etat.map != NULL) {
        GR0_free_state(&etat); // Libérer la grille précédente si elle existe
    }
    GR0_initialize();
    return;
}



void highlight_cell(SDL_Renderer* renderer, int x, int y,TTF_Font* font) {
    if (x >= 0 && x < grid_size && y >= 0 && y < grid_size) {
        int cell_x = GRID_OFFSET_X + x * CELL_SIZE;
        int cell_y = GRID_OFFSET_Y + y * CELL_SIZE;

        // Halo autour de la cellule, légèrement plus grand que la cellule
        SDL_Rect halo = {
            cell_x - 4,  // léger agrandissement
            cell_y - 4,  // léger agrandissement
            CELL_SIZE + 8,  // un peu plus grand que la cellule
            CELL_SIZE + 8   // un peu plus grand que la cellule
        };

        // Définir une couleur jaune pour le halo
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 100, 150);  // Jaune léger avec transparence
        SDL_RenderFillRect(renderer, &halo);

        // Redessine la cellule avec ses couleurs habituelles
        SDL_Color c = colors[get_map_value(&etat, x, y)];
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        SDL_Rect cell = {
            cell_x,
            cell_y,
            CELL_SIZE - 2,  // Légère réduction pour mieux voir l'effet halo
            CELL_SIZE - 2   // Légère réduction pour mieux voir l'effet halo
        };
        SDL_RenderFillRect(renderer, &cell);

        // Optionnel : afficher un texte pour indiquer le joueur, si c'est une cellule de joueur
        if (get_map_value(&etat,x,y) == PLAYER1_COLOR) {
            draw_text(renderer, font, "1", (SDL_Color){0, 0, 0, 255},
                      cell.x + cell.w / 2, cell.y + cell.h / 2);
        } else if (get_map_value(&etat,x,y) == PLAYER2_COLOR) {
            draw_text(renderer, font, "2", (SDL_Color){0, 0, 0, 255},
                      cell.x + cell.w / 2, cell.y + cell.h / 2);
        }
    }
}



void handle_grid_click(int mouseX, int mouseY, Queue* moves) {
    int gy = (mouseX - GRID_OFFSET_X) / CELL_SIZE;
    int gx = (mouseY - GRID_OFFSET_Y) / CELL_SIZE;
    resetQueues(moves);
    GR0_get_move_available(&etat, current_player, moves);
    printf("Click sur la case (%d,%d)\n", gx, gy);
    int coup_dispo = 0;
    for(int i=0;i<7;i++){
        if(isinQueue(&moves[i],(int[2]){gx,gy})){
            coup_dispo = i;
            printf("Case (%d,%d) disponible pour le joueur, couleur %d %d\n", gx, gy, current_player, i);
            break;
        }
    }

    if (coup_dispo!=0) {
        GR0_step(&etat, &moves[coup_dispo], current_player);
        GR0_plot(&etat);
        printf("Case (%d,%d) changée par le joueur %d\n", gx, gy, current_player);
        int fin = GR0_partie_finie(&etat);
        if (fin != 0) {
            printf("Partie terminée ! Joueur %d a gagné.\n", fin);
        }
        printf("agent %d\n", agent);
        current_player = (current_player == 1) ? 2 : 1; // Passer au joueur suivant
        if(current_player == 2 && agent!=-1){
            resetQueues(moves);
            GR0_get_move_available(&etat, 2, moves);
            Color coup= func_array[agent](&etat, 2);
            if (coup == -1) {
                printf("L'adversaire abandonne\n");
            } else {
                printf("Coup de l'agent %d : (%d, %d)\n", agent, gx, gy);
                GR0_step(&etat, &moves[coup], 2);
                GR0_plot(&etat);
                int fin = GR0_partie_finie(&etat);
                if (fin != 0) {
                    printf("Partie terminée ! Joueur %d a gagné.\n", fin);
                }
                current_player = 1;
            }
        } 
    }
}



int in_rect(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}



void draw_button(SDL_Renderer* renderer, SDL_Rect rect, const char* label, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}



void draw_slider(SDL_Renderer* renderer, int value) {
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



void draw_menu(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Rect startBtn = {300, 150, 200, 60};
    SDL_Rect opponentBtn = {300, 240, 200, 60};
    SDL_Rect quitBtn  = {300, 330, 200, 60};

    draw_button(renderer, startBtn, "", (SDL_Color){0, 200, 0, 255});
    draw_button(renderer, opponentBtn, "", (SDL_Color){0, 0, 200, 255});
    draw_button(renderer, quitBtn, "", (SDL_Color){200, 0, 0, 255});

    draw_text(renderer, font, "Démarrer", (SDL_Color){255,255,255,255}, startBtn.x + startBtn.w/2, startBtn.y + startBtn.h/2);
    draw_text(renderer, font, "Adversaire", (SDL_Color){255,255,255,255}, opponentBtn.x + opponentBtn.w/2, opponentBtn.y + opponentBtn.h/2);
    draw_text(renderer, font, "Quitter", (SDL_Color){255,255,255,255}, quitBtn.x + quitBtn.w/2, quitBtn.y + quitBtn.h/2);
    
    draw_text(renderer, font, "Taille de la grille :", (SDL_Color){255, 255, 255, 255}, WINDOW_WIDTH / 2, SLIDER_Y - 30);
    draw_slider(renderer, grid_size);

    char buf[20];
    snprintf(buf, sizeof(buf), "%d x %d", grid_size, grid_size);
    draw_text(renderer, font, buf, (SDL_Color){255, 255, 255, 255}, WINDOW_WIDTH / 2, SLIDER_Y + 30);
}



void draw_hovered_cell(SDL_Renderer* renderer, int mouseX, int mouseY) {
    int gy = (mouseX - GRID_OFFSET_X) / CELL_SIZE;
    int gx = (mouseY - GRID_OFFSET_Y) / CELL_SIZE;

    if (gx >= 0 && gx < grid_size && gy >= 0 && gy < grid_size) {
        SDL_Color c = colors[get_map_value(&etat,gx,gy)];
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 125);
        Queue network;
        initQueue(&network);
        GR0_get_network(&etat, (int[2]){gx,gy}, &network, NULL);
        while(network.length!=0){
            int pos[2];
            dequeue(&network, pos);
            SDL_Rect cell = {
                GRID_OFFSET_X + pos[1] * CELL_SIZE - 2,
                GRID_OFFSET_Y + pos[0] * CELL_SIZE - 2,
                CELL_SIZE + 6,
                CELL_SIZE + 6
            };
            SDL_RenderFillRect(renderer, &cell);
    
            // Bordure blanche en surbrillance
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
            SDL_RenderDrawRect(renderer, &cell);
            
        }
        
        freeQueue(&network);
    }
}



void draw_turn_info(SDL_Renderer* renderer, TTF_Font* font, int player) {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Tour du joueur %d", player);

    draw_text(renderer, font, buffer, (SDL_Color){255, 255, 255, 255},
              WINDOW_WIDTH / 2, GRID_OFFSET_Y - 20);
}

void draw_grid(SDL_Renderer* renderer, TTF_Font* font) {
    if (etat.map == NULL) {
        SDL_Log("Erreur : la grille n'est pas initialisée.");
        return;
    }

    for (int y = 0; y < grid_size; y++) {
        for (int x = 0; x < grid_size; x++) {
            highlight_cell(renderer, 3, 5,font);
            SDL_Color c = colors[get_map_value(&etat,y,x)];
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

            SDL_Rect cell = {
                GRID_OFFSET_X + x * CELL_SIZE + CELL_PADDING / 2,
                GRID_OFFSET_Y + y * CELL_SIZE + CELL_PADDING / 2,
                CELL_SIZE - CELL_PADDING,
                CELL_SIZE - CELL_PADDING
            };
            SDL_RenderFillRect(renderer, &cell);

            // Afficher "1" ou "2" si c’est une cellule de joueur
            if (get_map_value(&etat,y,x) == PLAYER1_COLOR) {
                draw_text(renderer, font, "1", (SDL_Color){0, 0, 0, 255},
                    cell.x + cell.w / 2, cell.y + cell.h / 2);
            } else if (get_map_value(&etat,y,x) == PLAYER2_COLOR) {
                draw_text(renderer, font, "2", (SDL_Color){0, 0, 0, 255},
                    cell.x + cell.w / 2, cell.y + cell.h / 2);
            }
        }
    }
}




int main(int argc, char** argv) {
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
    init_grid();

    SDL_Event event;
    int running = 1;
    Queue moves[7];
    initQueues(moves);
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
                            init_grid();
                        }
                    }
                
                    // Autres conditions pour les boutons (menu, quitter, etc.)
                if (state == MENU) {
                    SDL_Rect startBtn = {300, 150, 200, 60};
                    SDL_Rect opponentBtn = {300, 240, 200, 60};
                    SDL_Rect quitBtn = {300, 330, 200, 60};
                
                    if (in_rect(x, y, startBtn)) {
                        state = GAME;
                    } else if (in_rect(x, y, opponentBtn)) {
                        state = SELECT_OPPONENT;
                    } else if (in_rect(x, y, quitBtn)) {
                        running = 0;
                    }
                
                } else if (state == SELECT_OPPONENT) {
                    for (int i = 0; i < 8; ++i) {
                        SDL_Rect btn = {50 + (i % 3) * 250, 100 + (i / 3) * 100, 220, 100};
                        if (in_rect(x, y, btn)) {
                            printf("Agent : %s\n" , agent_names[i]);
                            agent = i;
                            state = MENU;
                        }
                    }
            
                } else if (state == GAME) {
                    handle_grid_click(x, y, moves);
                    
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (state == MENU) {
            draw_menu(renderer, font);
        } else if (state == GAME) {
            draw_turn_info(renderer, font, current_player);
            draw_grid(renderer, font);
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            draw_hovered_cell(renderer, mx, my);
        } else if (state == SELECT_OPPONENT) {
            for (int i = 0; i < 8; ++i) {
                SDL_Rect btn = {50 + (i % 3) * 250, 100 + (i / 3) * 100, 220, 100};
                draw_button(renderer, btn, "", (SDL_Color){0, 200, 200, 255});
                char label[20];
                snprintf(label, sizeof(label), "%s", agent_names[i]);
                draw_text(renderer, font, label, (SDL_Color){255, 255, 255, 255},
                          btn.x + btn.w / 2, btn.y + btn.h / 2);
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
    freeQueues(moves);
    GR0_free_state(&etat);
    return 0;
}
