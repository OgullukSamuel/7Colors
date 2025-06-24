#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "../head/GR0_queue.h"

int GR0_random_bit_index(uint8_t n);
int GR0_get_random_scalar(int min, int max);

float GR0_clip(float value, float min, float max) ;

void GR0_time_function(const char* function_name, void (*function_to_time)(void));


uint8_t GR0_condenser(Queue* moves);
void GR0_decondenser(uint8_t condenser,int bits[7]);

float GR0_exp_approx(float x);
float GR0_tanh_approx(float x);

#endif