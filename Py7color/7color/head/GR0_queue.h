#ifndef QUEUE_H
#define QUEUE_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_INIT_CAPACITY 5 // capacité initiale de la queue ( on double la capacité à chaque dépassement)

typedef struct {
    int* items;               
    int* sorted_values;        
    int front;
    int rear;                  
    int length;                
    int capacity;           
} Queue;

int GR0_injection(int* values);

void GR0_initQueue(Queue* q);
void GR0_initQueues(Queue* q);

void GR0_enqueue(Queue* q, int value[2]);
void GR0_dequeue(Queue* q, int value[2]);

void GR0_displayQueue(Queue* q);
void GR0_displayQueues(Queue* q);

void GR0_resetQueue(Queue* q);
void GR0_resetQueues(Queue* q);

void GR0_freeQueue(Queue* q); 
void GR0_freeQueues(Queue* q);

void GR0_resizeQueue(Queue* q);

int GR0_isinQueue(Queue* q, int value[2]);

void GR0_insert_sorted(Queue* q, int value[2]);


void GR0_concatenateQueue(Queue* queue1, Queue* queue2);
void GR0_copyQueue(Queue* original,Queue* copy);

void GR0_testQueue();


#endif