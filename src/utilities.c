#include "../head/utilities.h"


int GR0_random_bit_index(uint8_t n) {
    if (n == 0) return -1; // Aucun bit à 1

    int indices[8]; // Tableau pour stocker les indices des bits à 1
    int count = 0;

    // Parcours des 8 bits pour récupérer les indices des bits à 1
    for (int i = 0; i < 8; i++) {
        if (n & (1 << i)) { // Vérifie si le bit i est à 1
            indices[count++] = i;
        }
    }

    // Choisir un indice au hasard parmi ceux stockés
    return indices[rand() % count];
}



uint8_t GR0_condenser(Queue* moves){
	uint8_t result = 0;
	for(int i=0;i<7;i++){
		result |= (moves[i].length > 0) << i;
	}
	return(result);
}

void GR0_decondenser(uint8_t condenser,int bits[7]){
	for (int i = 0; i < 7; i++) {
		bits[i] = (condenser >> i) & 1;
	}
}	


float exp_approx(float x){
	return(1+x*(1+x/2*(1+x/3*(1+x/4))));
}

float tanh_approx(float x){
	return(x*(27+x*x)/(27+9*x*x));
}