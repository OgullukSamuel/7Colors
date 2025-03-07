#ifndef QUEUE_H
#define QUEUE_H


#define SIZE 10000  // Taille maximale de la queue

typedef struct {
    int items[SIZE][2];         // File FIFO stockant des tableaux [x, y]
    int sorted_values[SIZE];    // Liste triée pour la recherche binaire (basée sur l'injection)
    int front;
    int rear;            // Indices pour la gestion de la queue
    int length;                 // Nombre d'éléments dans la queue
} Queue;

void enqueue(Queue* q, int* value);
void dequeue(Queue* q, int* value);
void displayQueue(Queue* q);
void initQueue(Queue* q) ;
int isinQueue(Queue* q, int value[2]) ;
void insert_sorted(Queue* q, int (*value)) ;
//int injection(int* values);
#endif