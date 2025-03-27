#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../head/queue.h"
int GR0_random_bit_index(uint8_t n);

uint8_t GR0_condenser(Queue* moves);



float exp_approx(float x);
float tanh_approx(float x);

void GR0_decondenser(uint8_t condenser,int bits[7]);
#endif