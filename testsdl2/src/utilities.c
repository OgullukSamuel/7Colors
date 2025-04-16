#include "../head/utilities.h"


int GR0_random_bit_index(uint8_t n) {
    if (n == 0) return -1;

    int indices[8];
    int count = 0;

    for (int i = 0; i < 8; i++) {
        if (n & (1 << i)) {
            indices[count++] = i;
        }
    }

    return indices[rand() % count];
}

float clip(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

inline uint8_t GR0_condenser(Queue* moves){
	uint8_t result = 0;
	for(int i=0;i<7;i++){
		result |= (moves[i].length > 0) << i;
	}
	return(result);
}

inline void GR0_decondenser(uint8_t condenser,int bits[7]){
	for (int i = 0; i < 7; i++) {
		bits[i] = (condenser >> i) & 1;
	}
}	

void time_function(const char* function_name, void (*function_to_time)(void)) {
    clock_t start_time = clock();
    function_to_time();
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / (10*CLOCKS_PER_SEC);
    printf("Execution time of %s: %.6f seconds\n", function_name, elapsed_time);
}

int GR0_get_random_scalar(int min, int max){
    if (min > max) return -1;
    return min + rand() % (max - min + 1);
}

inline float exp_approx(float x){
	return(1/(1-x*(1-x/2)));
}

inline float tanh_approx(float x){
	return(x*(27+x*x)/(27+9*x*x));
}