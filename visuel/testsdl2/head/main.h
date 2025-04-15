#ifndef MAINS_H
#define MAINS_H
#include "../head/GameState.h"
#include "../head/display.h"
#include "../head/queue.h"
#include "../head/map_functions.h"
#include "../head/utilities.h"
#include "../head/Agents.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


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
#define GRID_OFFSET_X 100
#define GRID_OFFSET_Y 50
#define COLOR_COUNT 9
#define PLAYER1_COLOR 1
#define PLAYER2_COLOR 2
#define NUM_AGENT 9

int main(int argc, char** argv);
void GR0_initialize();

void draw_grid(SDL_Renderer* renderer, TTF_Font* font);

void draw_turn_info(SDL_Renderer* renderer, TTF_Font* font, int player);
void highlight_cell(SDL_Renderer* renderer, int x, int y,TTF_Font* font);
void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, int x, int y);
void draw_menu(SDL_Renderer* renderer, TTF_Font* font);
void draw_hovered_cell(SDL_Renderer* renderer, int x, int y);

void draw_slider(SDL_Renderer* renderer, int value);
void draw_slider_knob(SDL_Renderer* renderer, int x, int y, int radius);
#endif