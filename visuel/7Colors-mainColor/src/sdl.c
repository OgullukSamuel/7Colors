#include "../head/GameState.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 600
#define HEIGHT 400
#define MAX_TEXT 100
#define GRID_SIZE 6
#define CELL_SIZE 50

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;
SDL_Color textColor = {255, 255, 255, 255}; // Blanc

char inputText[MAX_TEXT] = "";
int running = 1;
int questionAnswered = 0;



SDL_Color colors[7] = {
    {255, 0, 0, 255},    // Rouge
    {0, 255, 0, 255},    // Vert
    {0, 0, 255, 255},    // Bleu
    {255, 255, 0, 255},  // Jaune
    {255, 165, 0, 255},  // Orange
    {128, 0, 128, 255},  // Violet
    {0, 255, 255, 255}   // Cyan
};
const char *correctAnswer = "bleu";
void renderText(const char *text, int yOffset) {
    if (!font || strlen(text) == 0) return;
    
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (!textSurface) {
        printf("Erreur création surface texte: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        printf("Erreur création texture texte: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    SDL_Rect textRect = {50, HEIGHT / 2 + yOffset, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void handleClick(int x, int y) {
    int gridX = (x - 50) / CELL_SIZE;
    int gridY = (y - 50) / CELL_SIZE;
    
    if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE) {
        printf("Case cliquée : (%d, %d), Couleur: %d\n", gridX, gridY, get_map_value(state, gridX, gridY));
    }
}

void handleInput(SDL_Event event) {
    if (event.type == SDL_TEXTINPUT) {
        if (strlen(inputText) < MAX_TEXT - 1) {
            strcat(inputText, event.text.text);
        }
    } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0) {
            inputText[strlen(inputText) - 1] = '\0';
        } else if (event.key.keysym.sym == SDLK_RETURN) {
            if (strcmp(inputText, correctAnswer) == 0) {
                questionAnswered = 1;
                initGrid();
            } else {
                printf("Réponse incorrecte. Réessayez.\n");
            }
            inputText[0] = '\0';
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN && questionAnswered) {
        handleClick(event.button.x, event.button.y);
    }
}

void renderGrid() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            SDL_Rect cell = {50 + j * CELL_SIZE, 50 + i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_Color color = colors[get_map_value(state, i, j)];
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
            SDL_RenderFillRect(renderer, &cell);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }
}

void gameLoop() {
    SDL_Event event;
    SDL_StartTextInput();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            handleInput(event);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (!questionAnswered) {
            renderText("Quelle IA souhaite tu bagarre", -50);
            renderText(inputText, 0);
        } else {
            renderGrid();
        }

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur init SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("Erreur init SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    
    window = SDL_CreateWindow("7 Colors", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur création fenêtre: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur création renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 24);
    if (!font) {
        printf("Erreur chargement police: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    gameLoop();

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}