#ifndef SDL_H
#define SDL_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../head/queue.h"
#include "../head/GameState.h"
#include "../head/Agents.h"
#include "../head/utilities.h"
#include "../head/map_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
typedef enum { MENU, GAME, SELECT_OPPONENT } etat_jeu;

typedef struct {
    SDL_Renderer* renderer;
    SDL_Color color;
    int pos[2];
} CellRenderData;
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define SLIDER_X 250
#define SLIDER_Y 500
#define SLIDER_WIDTH 300
#define SLIDER_HEIGHT 10
#define SLIDER_KNOB_RADIUS 10

#define GRID_MIN 4
#define GRID_MAX 15
#define CELL_SIZE 60
#define CELL_PADDING 8
#define GRID_OFFSET_X 250
#define GRID_OFFSET_Y 50
#define COLOR_COUNT 9
#define PLAYER1_COLOR 1
#define PLAYER2_COLOR 2
#define NUM_AGENT 9


SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color);
void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, int x, int y);
int in_rect(int x, int y, SDL_Rect rect);

void update_cursor(GameState* etat,int current_player, int* cursor_position, int cursor_active);
void handle_grid_click(int mouseX, int mouseY, Queue* moves,int* current_player, GameState* etat, int agent, int* winner, int* cursor_position, int cursor_active,func_ptr FUNC_ARRAY[NUM_AGENT]) ;
void draw_button(SDL_Renderer* renderer, SDL_Rect rect, const char* label, SDL_Color color);
void draw_slider(SDL_Renderer* renderer, int value);
void draw_hovered_cell(SDL_Renderer* renderer, int mouseX, int mouseY, SDL_Color colors[COLOR_COUNT],GameState* etat);

void draw_game_controls(SDL_Renderer* renderer, TTF_Font* font,int* cursor_active, int* winner);

void draw_menu(SDL_Renderer* renderer, TTF_Font* font, int grid_size);
void draw_cursor(SDL_Renderer* renderer, int position, int cursor_active, SDL_Color colors[COLOR_COUNT]);
void draw_turn_info(SDL_Renderer* renderer, TTF_Font* font, int player);

void draw_grid(SDL_Renderer* renderer, TTF_Font* font, GameState* etat, SDL_Color colors[COLOR_COUNT]);
void draw_slider_knob(SDL_Renderer* renderer, int x, int y, int radius);

int visual_main();
#endif