#ifndef SDL_H
#define SDL_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../head/GR0_queue.h"
#include "../head/GR0_GameState.h"
#include "../head/GR0_Agents.h"
#include "../head/GR0_utilities.h"
#include "../head/GR0_map_functions.h"
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
#define NUM_AGENTS 9


SDL_Texture* GR0_renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color);
void GR0_draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, int x, int y);
int GR0_in_rect(int x, int y, SDL_Rect rect);

void GR0_update_cursor(GameState* etat,int current_player, int* cursor_position, int cursor_active);
void GR0_handle_grid_click(int mouseX, int mouseY, Queue* moves, int* current_player, GameState* etat, int agent, int* winner, int* cursor_position, int cursor_active, func_ptr FUNC_ARRAY[NUM_AGENTS], int* swap_sides,int* swapchoice,int* hint) ;

void GR0_draw_rounded_rect(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color, int radius);
void GR0_draw_button(SDL_Renderer* renderer, SDL_Rect rect, const char* label, SDL_Color color);
void GR0_draw_cursor(SDL_Renderer* renderer, int position, int cursor_active, SDL_Color colors[COLOR_COUNT]);
void GR0_draw_slider(SDL_Renderer* renderer, int value);

void GR0_initialize_menu_textures(SDL_Renderer* renderer, TTF_Font* font);
void GR0_draw_menu(SDL_Renderer* renderer, TTF_Font* font, int grid_size);
void GR0_free_menu_textures();

void GR0_draw_hovered_cell(SDL_Renderer* renderer, int mouseX, int mouseY, SDL_Color colors[COLOR_COUNT],GameState* etat, int player,int* position,Queue* hover_network,Queue shots_available[7], int* swap_sides,int* swapchoice);

void GR0_draw_turn_info(SDL_Renderer* renderer, TTF_Font* font, int player);
void GR0_draw_grid(SDL_Renderer* renderer, TTF_Font* font, GameState* etat, SDL_Color colors[COLOR_COUNT], int* swap_sides,int* swapchoice);
void GR0_display_hint(SDL_Renderer* renderer,GameState* etat, int current_player,int* swapchoice,int* swap_sides);

void GR0_draw_game_controls(SDL_Renderer* renderer, TTF_Font* font, int* cursor_active, int* winner,int* swapchoice,int* hint);


void GR0_initialize_agent_textures(SDL_Renderer* renderer, TTF_Font* font);

int GR0_visual_main();
#endif