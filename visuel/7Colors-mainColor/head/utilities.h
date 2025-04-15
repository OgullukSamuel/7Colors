#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "../head/queue.h"

int GR0_random_bit_index(uint8_t n);


void time_function(const char* function_name, void (*function_to_time)(void));

int GR0_get_random_scalar(int min, int max);

uint8_t GR0_condenser(Queue* moves);

void GR0_decondenser(uint8_t condenser,int bits[7]);

float exp_approx(float x);
float tanh_approx(float x);

#endif