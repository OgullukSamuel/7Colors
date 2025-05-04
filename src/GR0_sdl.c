#include "../head/GR0_sdl.h"

char* GR0_agent_names[NUM_AGENTS] = {
    "Aléatoire      (elo:1100)",
    "  Glouton      (elo:1450)",
    "  Glouton      Heuristique    (elo 1500)",
    "  Minmax3      (elo 1700)",
    "  Minmax8      (elo 1900)",
    "Frontière5     (1000)",
    "Frontière5    Heuristique (elo 1800)",
    "Hégémonique    (elo 1500)",
    "Mixte          (elo 1550)"
};





func_ptr GR0_FUNC_ARRAY[NUM_AGENTS] = {&GR0_IA_Random, &GR0_Glouton,&GR0_Glouton_heuristique, &GR0_minmax3, &GR0_minmax6, &GR0_frontier_IA5, &GR0_frontier_IA5_heuristique,&GR0_hegemonique, &GR0_hegemonique_heuristique};

SDL_Color GR0_colors[COLOR_COUNT + 1] = {
    {0, 0, 0, 255},         // 0 - Ne sera pas utilisé (éviter le 0)
    {200, 165, 0, 255},     // 1 - Joueur 1 Orange
    {128, 0, 128, 255},          // 1 - Joueur 2 Violet
    {230, 0, 0, 255},       // 3 - Rouge
    {0, 200, 0, 255},       // 4 - Vert
    {0, 0, 200, 255},       // 5 - Bleu
    {200, 200, 0, 255},     // 6 - Jaune
    {240, 0, 240, 255},     // 7 - Magenta
    {0, 220, 220, 255},     // 8 - Cyan
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
    // Fonction pour dessiner du texte centré en x,y
    SDL_Texture* texture = GR0_renderText(renderer, font, text, color);
    if (!texture) return;

    int texW = 0, texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dst = {x - texW / 2, y - texH / 2, texW, texH};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

int GR0_in_rect(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}

void GR0_update_cursor(GameState* etat,int current_player, int* cursor_position, int cursor_active) {
    if (cursor_active) {
        float evaluation= GR0_minmax8_evaluation(etat,current_player);
        printf("Evaluation : %f\n", evaluation);
        float maxreward=etat->size*etat->size;
        *cursor_position = GR0_clip(evaluation*100/maxreward,-100,100);
    }
}

void GR0_handle_grid_click(int mouseX, int mouseY, Queue* moves, int* current_player, GameState* etat, int agent, int* winner, int* cursor_position, int cursor_active, func_ptr FUNC_ARRAY[NUM_AGENTS], int* swap_sides,int* swapchoice,int* hint) {
    int gy = (mouseX - GRID_OFFSET_X) / CELL_SIZE;
    int gx = (mouseY - GRID_OFFSET_Y) / CELL_SIZE;

    if (*swapchoice&&*swap_sides) {
        gx = etat->size - 1 - gx;
        gy = etat->size - 1 - gy;
    }

    if (gx < 0 || gx >= etat->size || gy < 0 || gy >= etat->size) {
        return;
    }

    GR0_resetQueues(moves);
    GR0_get_total_moves(etat, *current_player, moves);

    int coup_dispo = -1;
    for (int i = 0; i < 7; i++) {
        if (GR0_isinQueue(&moves[i], (int[2]){gx, gy})) {
            coup_dispo = i;
            break;
        }
    }

    if (coup_dispo != -1) {
        GR0_step(etat, &moves[coup_dispo], *current_player);
        GR0_update_cursor(etat, *current_player, cursor_position, cursor_active);
        *hint =0;
        int fin = GR0_partie_finie(etat);

        if (fin != 0) {
            if (fin != 3) {
                printf("Partie terminée ! Joueur %d a gagné.\n", fin);
            } else {
                printf("Partie nulle !\n");
            }
            *winner = fin;
        }

        *current_player = (*current_player == 1) ? 2 : 1;

        if(*swapchoice){*swap_sides = !(*swap_sides);}
        
        if (*current_player == 2 && agent != -1) {
            GR0_resetQueues(moves);
            GR0_get_move_available(etat, 2, moves);
            Color coup = FUNC_ARRAY[agent](etat, 2);
            if (coup == -1) {
                printf("L'adversaire abandonne. Joueur %d gagne.\n", 1);
                *winner = 1;
            } else {
                printf("Coup de l'agent %d : (%d, %d)\n", agent, gx, gy);
                GR0_step(etat, &moves[coup], 2);
                GR0_update_cursor(etat, *current_player, cursor_position, cursor_active);
                int fin = GR0_partie_finie(etat);
                if (fin != 0) {
                    if (fin != 3) {
                        printf("Partie terminée ! Joueur %d a gagné.\n", fin);
                    } else {
                        printf("Partie nulle !\n");
                    }
                    *winner = fin;
                }
                *current_player = 1;
            }
        }
    }
}

TTF_Font* global_font = NULL;

void GR0_draw_rounded_rect(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color, int radius) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    SDL_Rect central_rect = {rect.x + radius, rect.y, rect.w - 2 * radius, rect.h};
    SDL_RenderFillRect(renderer, &central_rect);


    SDL_Rect left_rect = {rect.x, rect.y + radius, radius, rect.h - 2 * radius};
    SDL_Rect right_rect = {rect.x + rect.w - radius, rect.y + radius, radius, rect.h - 2 * radius};
    SDL_RenderFillRect(renderer, &left_rect);
    SDL_RenderFillRect(renderer, &right_rect);

    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; 
            int dy = radius - h; 
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, rect.x + radius - dx, rect.y + radius - dy);
                SDL_RenderDrawPoint(renderer, rect.x + rect.w - radius + dx - 1, rect.y + radius - dy);
                SDL_RenderDrawPoint(renderer, rect.x + radius - dx, rect.y + rect.h - radius + dy - 1); 
                SDL_RenderDrawPoint(renderer, rect.x + rect.w - radius + dx - 1, rect.y + rect.h - radius + dy - 1);
            }
        }
    }
}

void GR0_draw_button(SDL_Renderer* renderer, SDL_Rect rect, const char* label, SDL_Color color) {
    static char lastLabel[100] = "";
    static SDL_Texture* labelTexture = NULL;

    if (strcmp(label, lastLabel) != 0) {
        if (labelTexture) SDL_DestroyTexture(labelTexture);
        labelTexture = GR0_renderText(renderer, global_font, label, (SDL_Color){0, 0, 0, 255});
        strncpy(lastLabel, label, sizeof(lastLabel) - 1);
    }

    int mx, my;
    SDL_GetMouseState(&mx, &my);

    int is_hovered = GR0_in_rect(mx, my, rect);
    if (is_hovered) {
        rect.x -= 5; 
        rect.y -= 5;
        rect.w += 10;
        rect.h += 10;
    }

    GR0_draw_rounded_rect(renderer, rect, color, 10);

    if (labelTexture) {
        int texW, texH;
        SDL_QueryTexture(labelTexture, NULL, NULL, &texW, &texH);
        SDL_Rect dst = {rect.x + rect.w / 2 - texW / 2, rect.y + rect.h / 2 - texH / 2, texW, texH};
        SDL_RenderCopy(renderer, labelTexture, NULL, &dst);
    }
}

void GR0_draw_cursor(SDL_Renderer* renderer, int position, int cursor_active, SDL_Color colors[COLOR_COUNT]) {
    if (!cursor_active) return;

    int cursor_min = -100;
    int cursor_max = 100;

    float norm = (float)(position - cursor_min) / (cursor_max - cursor_min);
    int cursor_y = GRID_OFFSET_Y + (int)(norm * (WINDOW_HEIGHT - GRID_OFFSET_Y * 2));

    int cursor_x = 30;
    int cursor_width = 20;
    SDL_Color c = colors[1];

    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255); // Bleu
    SDL_Rect blueZone = {cursor_x, GRID_OFFSET_Y, cursor_width, cursor_y - GRID_OFFSET_Y};
    SDL_RenderFillRect(renderer, &blueZone);
    c = colors[2];

    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255); // Rouge
    SDL_Rect redZone = {cursor_x, cursor_y, cursor_width, WINDOW_HEIGHT - cursor_y - GRID_OFFSET_Y};
    SDL_RenderFillRect(renderer, &redZone);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
    SDL_Rect line = {cursor_x, cursor_y - 1, cursor_width, 2}; // Ligne fine
    SDL_RenderFillRect(renderer, &line);

    int zero_y = GRID_OFFSET_Y + (WINDOW_HEIGHT - GRID_OFFSET_Y * 2) / 2; // Position verticale pour zéro
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Gris clair
    SDL_Rect zeroLine = {cursor_x - 10, zero_y - 1, cursor_width + 20, 2}; // Barre horizontale
    SDL_RenderFillRect(renderer, &zeroLine);

    if (global_font) {
        SDL_Color textColor = {255, 255, 255, 255};
        GR0_draw_text(renderer, global_font, "100", textColor, cursor_x + cursor_width / 2, GRID_OFFSET_Y - 10); // En haut
        GR0_draw_text(renderer, global_font, "-100", textColor, cursor_x + cursor_width / 2, WINDOW_HEIGHT - GRID_OFFSET_Y + 10); // En bas
    }
}

void GR0_draw_slider(SDL_Renderer* renderer, int value) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_Rect line = {SLIDER_X, SLIDER_Y, SLIDER_WIDTH, SLIDER_HEIGHT};
    SDL_RenderFillRect(renderer, &line);

    float norm = (float)(value - GRID_MIN) / (GRID_MAX - GRID_MIN);
    int knobX = SLIDER_X + (int)(norm * SLIDER_WIDTH);

    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    SDL_Rect knob = {knobX - SLIDER_KNOB_RADIUS, SLIDER_Y - SLIDER_KNOB_RADIUS + SLIDER_HEIGHT/2,SLIDER_KNOB_RADIUS * 2, SLIDER_KNOB_RADIUS * 2};
    SDL_RenderFillRect(renderer, &knob);
}


static SDL_Texture* startBtnTexture = NULL;
static SDL_Texture* opponentBtnTexture = NULL;
static SDL_Texture* quitBtnTexture = NULL;
static SDL_Texture* menuBackgroundTexture = NULL;

void GR0_initialize_menu_textures(SDL_Renderer* renderer, TTF_Font* font) {
    if (!startBtnTexture) {
        startBtnTexture = GR0_renderText(renderer, font, "Démarrer", (SDL_Color){0, 0, 0, 255});
    }
    if (!opponentBtnTexture) {
        opponentBtnTexture = GR0_renderText(renderer, font, "Adversaire", (SDL_Color){0, 0, 0, 255});
    }
    if (!quitBtnTexture) {
        quitBtnTexture = GR0_renderText(renderer, font, "Quitter", (SDL_Color){0, 0, 0, 255});
    }
}

void GR0_draw_menu(SDL_Renderer* renderer, TTF_Font* font, int grid_size) {
    SDL_Rect startBtn = {300, 150, 200, 60};
    SDL_Rect opponentBtn = {300, 240, 200, 60};
    SDL_Rect quitBtn = {300, 330, 200, 60};

    int mx, my;
    SDL_GetMouseState(&mx, &my);


    int is_hovered = GR0_in_rect(mx, my, startBtn);
    SDL_Rect startBtnHover = startBtn;
    if (is_hovered) {
        startBtnHover.x -= 5;
        startBtnHover.y -= 5;
        startBtnHover.w += 10;
        startBtnHover.h += 10;
    }
    GR0_draw_rounded_rect(renderer, startBtnHover, (SDL_Color){0, 200, 0, 255}, 10);
    if (startBtnTexture) {
        int texW, texH;
        SDL_QueryTexture(startBtnTexture, NULL, NULL, &texW, &texH);
        SDL_Rect dst = {startBtnHover.x + startBtnHover.w / 2 - texW / 2, startBtnHover.y + startBtnHover.h / 2 - texH / 2, texW, texH};
        SDL_RenderCopy(renderer, startBtnTexture, NULL, &dst);
    }


    is_hovered = GR0_in_rect(mx, my, opponentBtn);
    SDL_Rect opponentBtnHover = opponentBtn;
    if (is_hovered) {
        opponentBtnHover.x -= 5;
        opponentBtnHover.y -= 5;
        opponentBtnHover.w += 10;
        opponentBtnHover.h += 10;
    }
    GR0_draw_rounded_rect(renderer, opponentBtnHover, (SDL_Color){0, 0, 200, 255}, 10);
    if (opponentBtnTexture) {
        int texW, texH;
        SDL_QueryTexture(opponentBtnTexture, NULL, NULL, &texW, &texH);
        SDL_Rect dst = {opponentBtnHover.x + opponentBtnHover.w / 2 - texW / 2, opponentBtnHover.y + opponentBtnHover.h / 2 - texH / 2, texW, texH};
        SDL_RenderCopy(renderer, opponentBtnTexture, NULL, &dst);
    }


    is_hovered = GR0_in_rect(mx, my, quitBtn);
    SDL_Rect quitBtnHover = quitBtn;
    if (is_hovered) {
        quitBtnHover.x -= 5;
        quitBtnHover.y -= 5;
        quitBtnHover.w += 10;
        quitBtnHover.h += 10;
    }
    GR0_draw_rounded_rect(renderer, quitBtnHover, (SDL_Color){200, 0, 0, 255}, 10);
    if (quitBtnTexture) {
        int texW, texH;
        SDL_QueryTexture(quitBtnTexture, NULL, NULL, &texW, &texH);
        SDL_Rect dst = {quitBtnHover.x + quitBtnHover.w / 2 - texW / 2, quitBtnHover.y + quitBtnHover.h / 2 - texH / 2, texW, texH};
        SDL_RenderCopy(renderer, quitBtnTexture, NULL, &dst);
    }

    TTF_SetFontSize(font, 24);
    GR0_draw_text(renderer, font, "Taille de la grille :", (SDL_Color){255, 255, 255, 255}, WINDOW_WIDTH / 2, SLIDER_Y - 30);
    GR0_draw_slider(renderer, grid_size);
    char gridSizeText[20];
    snprintf(gridSizeText, sizeof(gridSizeText), "%d x %d", grid_size, grid_size);
    GR0_draw_text(renderer, font, gridSizeText, (SDL_Color){255, 255, 255, 255}, WINDOW_WIDTH / 2, SLIDER_Y + 50);
}

void GR0_free_menu_textures() {
    if (startBtnTexture) {
        SDL_DestroyTexture(startBtnTexture);
        startBtnTexture = NULL;
    }
    if (opponentBtnTexture) {
        SDL_DestroyTexture(opponentBtnTexture);
        opponentBtnTexture = NULL;
    }
    if (quitBtnTexture) {
        SDL_DestroyTexture(quitBtnTexture);
        quitBtnTexture = NULL;
    }
    if (menuBackgroundTexture) {
        SDL_DestroyTexture(menuBackgroundTexture);
        menuBackgroundTexture = NULL;
    }
}

void GR0_draw_hovered_cell(SDL_Renderer* renderer, int mouseX, int mouseY, SDL_Color colors[COLOR_COUNT], GameState* etat, int player, int* position, Queue* hover_network, Queue shots_available[7], int* swap_sides,int* swapchoice) {
    int gy = (mouseX - GRID_OFFSET_X) / CELL_SIZE;
    int gx = (mouseY - GRID_OFFSET_Y) / CELL_SIZE;

    // Apply 180-degree rotation if swap_sides is enabled
    if (*swapchoice && *swap_sides) {
        gx = etat->size - 1 - gx;
        gy = etat->size - 1 - gy;
    }

    if (gx < 0 || gx >= etat->size || gy < 0 || gy >= etat->size) {
        return;
    }

    if (gx != position[0] || gy != position[1]) {
        GR0_resetQueue(hover_network);
        GR0_get_network(etat, (int[2]){gx, gy}, hover_network, NULL);
        position[0] = gx;
        position[1] = gy;
    }
    Queue hovernet;
    GR0_copyQueue(hover_network,&hovernet);
    SDL_Color c = colors[get_map_value(etat, gx, gy)];
    int length = hover_network->length;

    int positions[length][2];
    for (int i = 0; i < length; i++) {
        GR0_dequeue(&hovernet, positions[i]);
    }

    for (int i = 0; i < length; i++) {
        int draw_x = positions[i][1];
        int draw_y = positions[i][0];

        if (*swapchoice &&*swap_sides) {
            draw_x = etat->size - 1 - positions[i][1];
            draw_y = etat->size - 1 - positions[i][0];
        }

        SDL_Rect cell = {
            GRID_OFFSET_X + draw_x * CELL_SIZE + 2,
            GRID_OFFSET_Y + draw_y * CELL_SIZE + 2,
            CELL_SIZE - 3,
            CELL_SIZE - 3
        };
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 50);
        SDL_RenderFillRect(renderer, &cell);
    }
}

void GR0_draw_turn_info(SDL_Renderer* renderer, TTF_Font* font, int player) {
    static SDL_Texture* turnTextTexture = NULL;
    static int lastPlayer = -1;

    if (player != lastPlayer) {
        if (turnTextTexture) SDL_DestroyTexture(turnTextTexture);

        char buffer[50];
        snprintf(buffer, sizeof(buffer), "Tour du joueur %d", player);
        turnTextTexture = GR0_renderText(renderer, font, buffer, (SDL_Color){255, 255, 255, 255});
        lastPlayer = player;
    }

    if (turnTextTexture) {
        int texW, texH;
        SDL_QueryTexture(turnTextTexture, NULL, NULL, &texW, &texH);
        SDL_Rect dst = {((WINDOW_WIDTH+ GRID_OFFSET_X/2 )/ 2) - texW / 2, GRID_OFFSET_Y - 20 - texH / 2, texW, texH};
        SDL_RenderCopy(renderer, turnTextTexture, NULL, &dst);
    }
}

void GR0_draw_grid(SDL_Renderer* renderer, TTF_Font* font, GameState* etat, SDL_Color colors[COLOR_COUNT], int* swap_sides,int* swapchoice) {
    if (etat->map == NULL) {
        SDL_Log("Erreur : la grille n'est pas initialisée.");
        return;
    }

    for (int y = 0; y < etat->size; y++) {
        for (int x = 0; x < etat->size; x++) {
            int draw_x = x;
            int draw_y = y;

            // Apply 180-degree rotation if swap_sides is enabled
            if (*swapchoice && *swap_sides) {
                draw_x = etat->size - 1 - x;
                draw_y = etat->size - 1 - y;
            }

            SDL_Color c = colors[get_map_value(etat, y, x)];
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

            SDL_Rect cell = {
                GRID_OFFSET_X + draw_x * CELL_SIZE + CELL_PADDING / 2,
                GRID_OFFSET_Y + draw_y * CELL_SIZE + CELL_PADDING / 2,
                CELL_SIZE - CELL_PADDING,
                CELL_SIZE - CELL_PADDING
            };
            SDL_RenderFillRect(renderer, &cell);

            if (get_map_value(etat, y, x) == PLAYER1_COLOR) {
                GR0_draw_text(renderer, font, "1", (SDL_Color){0, 0, 0, 255},
                    cell.x + cell.w / 2, cell.y + cell.h / 2);
            } else if (get_map_value(etat, y, x) == PLAYER2_COLOR) {
                GR0_draw_text(renderer, font, "2", (SDL_Color){0, 0, 0, 255},
                    cell.x + cell.w / 2, cell.y + cell.h / 2);
            }
        }
    }
}

void GR0_display_hint(SDL_Renderer* renderer, GameState* etat, int current_player, int* swapchoice, int* swap_sides) {
    Queue moves[7];
    GR0_initQueues(moves);
    GR0_get_total_moves(etat, current_player, moves);
    Color shot = GR0_minmax8(etat, current_player);
    if (shot == -1) {
        printf("Abandon %d.\n", current_player);
        return;
    }

    int length = moves[shot].length;

    int positions[length][2];
    for (int i = 0; i < length; i++) {
        GR0_dequeue(&moves[shot], positions[i]);
    }

    GR0_freeQueues(moves);

    SDL_Color c = GR0_colors[current_player];
    SDL_Color c2 = GR0_colors[get_map_value(etat, positions[0][0], positions[0][1])];
    for (int i = 0; i < length; i++) {
        int draw_x = positions[i][1];
        int draw_y = positions[i][0];

        if (*swapchoice && *swap_sides) {
            draw_x = etat->size - 1 - positions[i][1];
            draw_y = etat->size - 1 - positions[i][0];
        }

        SDL_Rect outerRect = {
            GRID_OFFSET_X + draw_x * CELL_SIZE - 2,
            GRID_OFFSET_Y + draw_y * CELL_SIZE - 2,
            CELL_SIZE + 4,
            CELL_SIZE + 4
        };
        SDL_Rect innerRect = {
            GRID_OFFSET_X + draw_x * CELL_SIZE + 2,
            GRID_OFFSET_Y + draw_y * CELL_SIZE + 2,
            CELL_SIZE - 3,
            CELL_SIZE - 3
        };

        // Draw the outer rectangle (halo)
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 100); // Semi-transparent color
        SDL_RenderFillRect(renderer, &outerRect);

        // Clear the inner rectangle to create the border effect
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Transparent (background color)
        SDL_RenderFillRect(renderer, &innerRect);

        // Draw the actual square
        SDL_SetRenderDrawColor(renderer, c2.r, c2.g, c2.b, 255); // Solid color
        SDL_RenderFillRect(renderer, &innerRect);
    }

    return;

}

void GR0_draw_game_controls(SDL_Renderer* renderer, TTF_Font* font, int* cursor_active, int* winner, int* swapchoice, int* hint) {
    int button_offset_x = GRID_OFFSET_X - 180;

    SDL_Rect cursorToggleBtn = {button_offset_x, GRID_OFFSET_Y, 150, 50};
    SDL_Rect backToMenuBtn = {button_offset_x, GRID_OFFSET_Y + 70, 150, 50}; // Adjusted spacing
    SDL_Rect hintBtn = {button_offset_x, GRID_OFFSET_Y + 140, 150, 50}; // Adjusted spacing
    SDL_Rect swapSidesBtn = {button_offset_x, GRID_OFFSET_Y + 210, 150, 50}; // Adjusted spacing
    SDL_Rect resignBtn = {button_offset_x, WINDOW_HEIGHT - GRID_OFFSET_Y - 50, 150, 50}; // Bottom-left

    GR0_draw_button(renderer, cursorToggleBtn, "Évaluation",
                    *cursor_active ? (SDL_Color){0, 200, 0, 255} : (SDL_Color){200, 0, 0, 255});

    GR0_draw_button(renderer, backToMenuBtn, "Retour Menu", (SDL_Color){200, 200, 0, 255});
    GR0_draw_button(renderer, resignBtn, "Abandonner", (SDL_Color){200, 0, 0, 255});
    GR0_draw_button(renderer, hintBtn, "Hint", *hint ? (SDL_Color){200, 0, 0, 255} : (SDL_Color){0, 200, 200, 255});
    GR0_draw_button(renderer, swapSidesBtn, "Rotation",
                    *swapchoice ? (SDL_Color){0, 200, 0, 255} : (SDL_Color){200, 0, 0, 255});

    if (*winner != 0) {
        SDL_Rect replayBtn = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 30, 400, 60};
        char buffer[50];
        if (*winner != 3) {
            snprintf(buffer, sizeof(buffer), "Le joueur %d a gagné ! Rejouer ?", *winner);
        } else {
            snprintf(buffer, sizeof(buffer), "Partie nulle ! Rejouer ?");
        }
        GR0_draw_button(renderer, replayBtn, buffer, (SDL_Color){0, 200, 0, 255});
    }
}

// Add a static array to cache textures for agent names
static SDL_Texture* agent_name_textures[8] = {NULL};

// Update the function to handle multi-line text rendering with a new font
void GR0_initialize_agent_textures(SDL_Renderer* renderer, TTF_Font* font) {
    TTF_Font* name_font = TTF_OpenFont("verdana.ttf", 20); // Use a different font for adversary names
    if (!name_font) {
        SDL_Log("Erreur chargement police (verdana.ttf) : %s", TTF_GetError());
        name_font = font; // Fallback to the global font if loading fails
    }

    for (int i = 0; i < 8; ++i) {
        if (agent_name_textures[i]) SDL_DestroyTexture(agent_name_textures[i]);

        // Split the agent name into multiple lines
        char line1[20] = {0}, line2[20] = {0}, line3[20] = {0};
        snprintf(line1, sizeof(line1), "%.15s", GR0_agent_names[i]); // First 15 characters
        if (strlen(GR0_agent_names[i]) > 15) {
            snprintf(line2, sizeof(line2), "%.15s", GR0_agent_names[i] + 15); // Next 15 characters
        }
        if (strlen(GR0_agent_names[i]) > 30) {
            snprintf(line3, sizeof(line3), "%.15s", GR0_agent_names[i] + 30); // Remaining characters
        }

        // Create a surface for each line and combine them into a single texture
        SDL_Surface* surface1 = TTF_RenderUTF8_Blended(name_font, line1, (SDL_Color){255, 255, 255, 255});
        SDL_Surface* surface2 = line2[0] ? TTF_RenderUTF8_Blended(name_font, line2, (SDL_Color){255, 255, 255, 255}) : NULL;
        SDL_Surface* surface3 = line3[0] ? TTF_RenderUTF8_Blended(name_font, line3, (SDL_Color){255, 255, 255, 255}) : NULL;

        int totalHeight = surface1->h + (surface2 ? surface2->h : 0) + (surface3 ? surface3->h : 0);
        int maxWidth = surface1->w;
        if (surface2 && surface2->w > maxWidth) maxWidth = surface2->w;
        if (surface3 && surface3->w > maxWidth) maxWidth = surface3->w;

        SDL_Surface* combinedSurface = SDL_CreateRGBSurface(0, maxWidth, totalHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        SDL_Rect dest = {0, 0, 0, 0};
        SDL_BlitSurface(surface1, NULL, combinedSurface, &dest);
        if (surface2) {
            dest.y += surface1->h;
            SDL_BlitSurface(surface2, NULL, combinedSurface, &dest);
        }
        if (surface3) {
            dest.y += surface2->h;
            SDL_BlitSurface(surface3, NULL, combinedSurface, &dest);
        }

        agent_name_textures[i] = SDL_CreateTextureFromSurface(renderer, combinedSurface);

        SDL_FreeSurface(surface1);
        if (surface2) SDL_FreeSurface(surface2);
        if (surface3) SDL_FreeSurface(surface3);
        SDL_FreeSurface(combinedSurface);
    }

    if (name_font != font) {
        TTF_CloseFont(name_font); // Close the custom font if it was loaded
    }
}

int GR0_visual_main() {
    int current_player = 1;
    int winner = 0;
    int cursor_active = 0;
    int cursor_position = 0;
    int grid_size = 8;
    int agent = -1;
    int position[2] = {0, 0};
    int swap_sides = 0;
    int swapchoice = 0;
    int hint = 0;
    GameState etat = {.map = NULL, .size = 0};
    Queue hovernet;
    GR0_initQueue(&hovernet);
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
    
    global_font = TTF_OpenFont("arial.ttf", 24); // Load the font once
    if (!global_font) {
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
    GR0_get_move_available(&etat,current_player,moves);
    int mx, my; // Declare mx and my at the beginning of the function
    GR0_initialize_menu_textures(renderer, global_font);
    while (running) {
        SDL_GetMouseState(&mx, &my); // Update mx and my at the start of each frame

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
                            100 + (i % 3) * 250, 
                            100 + (i / 3) * 140,
                            200,
                            100   
                        };
                        if (GR0_in_rect(x, y, btn)) {
                            printf("Agent : %s\n", GR0_agent_names[i]);
                            agent = i;
                            state = MENU;
                        }
                        GR0_draw_button(renderer, btn, "", (SDL_Color){0, 200, 200, 255});
                        char line1[20] = {0}, line2[20] = {0}, line3[20] = {0};
                        snprintf(line1, sizeof(line1), "%.10s", GR0_agent_names[i]);
                        if (strlen(GR0_agent_names[i]) > 10) {
                            snprintf(line2, sizeof(line2), "%.10s", GR0_agent_names[i] + 10);
                        }
                        if (strlen(GR0_agent_names[i]) > 20) {
                            snprintf(line3, sizeof(line3), "%.10s", GR0_agent_names[i] + 20);
                        }

                        GR0_draw_text(renderer, global_font, line1, (SDL_Color){255, 255, 255, 255},
                                  btn.x + btn.w / 2, btn.y + btn.h / 4);
                        if (strlen(line2) > 0) {
                            GR0_draw_text(renderer, global_font, line2, (SDL_Color){255, 255, 255, 255},
                                      btn.x + btn.w / 2, btn.y + btn.h / 2); // Ligne 2
                        }
                        if (strlen(line3) > 0) {
                            GR0_draw_text(renderer, global_font, line3, (SDL_Color){255, 255, 255, 255},
                                      btn.x + btn.w / 2, btn.y + 3 * btn.h / 4); // Ligne 3
                        }
                    }
                } else if (state == GAME) {
                    SDL_Rect cursorToggleBtn = {GRID_OFFSET_X - 180, GRID_OFFSET_Y, 150, 50};
                    SDL_Rect backToMenuBtn = {GRID_OFFSET_X - 180, GRID_OFFSET_Y + 70, 150, 50}; // Adjusted spacing
                    SDL_Rect hintBtn = {GRID_OFFSET_X - 180, GRID_OFFSET_Y + 140, 150, 50}; // Adjusted spacing
                    SDL_Rect swapSidesBtn = {GRID_OFFSET_X - 180, GRID_OFFSET_Y + 210, 150, 50}; // Adjusted spacing
                    SDL_Rect resignBtn = {GRID_OFFSET_X - 180, WINDOW_HEIGHT - GRID_OFFSET_Y - 50, 150, 50}; // Bottom-left

                    if (winner != 0) {
                        SDL_Rect replayBtn = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 30, 400, 60};
                        if (GR0_in_rect(x, y, replayBtn)) {
                            // Réinitialiser l'état du jeu
                            winner = 0;
                            current_player = 1;
                            cursor_active = 0;
                            cursor_position = 0;
                            swap_sides = 0;
                            swapchoice = 0;
                            hint = 0;
                            GR0_initialize(&etat, grid_size);
                            GR0_resetQueue(&hovernet);
                            GR0_initQueues(moves);
                            printf("Partie redémarrée.\n");
                        }
                    }

                    if (GR0_in_rect(x, y, cursorToggleBtn)) {
                        cursor_active = !cursor_active;
                        if (cursor_active) {
                            GR0_update_cursor(&etat, current_player, &cursor_position, cursor_active);
                        }
                    }
                    if (GR0_in_rect(x, y, backToMenuBtn)) {
                        state = MENU;
                        printf("Retour au menu principal.\n");
                    }
                    if (GR0_in_rect(x, y, hintBtn)) {
                        hint = 1;
                    }
                    if (GR0_in_rect(x, y, swapSidesBtn)) {
                        swapchoice = !swapchoice;
                    }

                    if (GR0_in_rect(x, y, resignBtn)) {
                        winner = (current_player == 1) ? 2 : 1; // L'autre joueur gagne
                        printf("Le joueur %d a abandonné. Le joueur %d gagne.\n", current_player, winner);
                    }
                    if (GR0_in_rect(x, y, hintBtn)) {
                        hint =1;
                    }
                    if (x >= GRID_OFFSET_X && x < GRID_OFFSET_X + etat.size * CELL_SIZE &&
                        y >= GRID_OFFSET_Y && y < GRID_OFFSET_Y + etat.size * CELL_SIZE){
                        GR0_handle_grid_click(x, y, moves, &current_player, &etat, agent, &winner, &cursor_position, cursor_active, GR0_FUNC_ARRAY, &swap_sides,&swapchoice,&hint);
                    }
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 59, 57, 74, 255);
        SDL_RenderClear(renderer);

        if (state == MENU) {
            GR0_draw_menu(renderer, global_font, grid_size);
        } else if (state == GAME) {

            GR0_draw_turn_info(renderer, global_font, current_player);
            GR0_draw_grid(renderer, global_font, &etat, GR0_colors, &swap_sides,&swapchoice);
            
            if(hint){
                GR0_display_hint(renderer,&etat,current_player,&swap_sides,&swapchoice);
            }
            
            GR0_draw_hovered_cell(renderer, mx, my, GR0_colors, &etat,current_player, position, &hovernet,moves, &swap_sides,&swapchoice);
            
            GR0_draw_cursor(renderer, cursor_position, cursor_active, GR0_colors);
            GR0_draw_game_controls(renderer, global_font, &cursor_active, &winner, &swapchoice,&hint);

        } else if (state == SELECT_OPPONENT) {
            if (!agent_name_textures[0]) {
                GR0_initialize_agent_textures(renderer, global_font);
            }

            for (int i = 0; i < 8; ++i) {
                SDL_Rect btn = {
                    100 + (i % 3) * 250, // Position horizontale (3 boutons par ligne)
                    100 + (i / 3) * 140, // Position verticale (adjusted spacing)
                    200, // Largeur
                    100   // Hauteur (increased height)
                };
                GR0_draw_button(renderer, btn, "", (SDL_Color){0, 200, 200, 255});

                if (agent_name_textures[i]) {
                    int texW, texH;
                    SDL_QueryTexture(agent_name_textures[i], NULL, NULL, &texW, &texH);
                    SDL_Rect dst = {btn.x + btn.w / 2 - texW / 2, btn.y + btn.h / 2 - texH / 2, texW, texH};
                    SDL_RenderCopy(renderer, agent_name_textures[i], NULL, &dst);
                }
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }

    TTF_CloseFont(global_font); // Close the font once
    global_font = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
    TTF_Quit();
    GR0_freeQueue(&hovernet);
    GR0_freeQueues(moves);
    GR0_free_state(&etat);

    // Free textures when exiting the program
    for (int i = 0; i < 8; ++i) {
        if (agent_name_textures[i]) {
            SDL_DestroyTexture(agent_name_textures[i]);
            agent_name_textures[i] = NULL;
        }
    }

    GR0_free_menu_textures();

    return 0;
}
