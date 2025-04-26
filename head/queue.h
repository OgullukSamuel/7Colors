#ifndef QUEUE_H
#define QUEUE_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> // For dynamic memory allocation

#define QUEUE_INIT_CAPACITY 5 

typedef struct {
    int* items;               // Dynamically allocated array for [x, y] pairs
    int* sorted_values;        // Dynamically allocated array for sorted values
    int front;
    int rear;                  // Indices for queue management
    int length;                // Number of elements in the queue
    int capacity;              // Maximum capacity of the queue
} Queue;

int GR0_injection(int* values);
void GR0_enqueue(Queue* q, int value[2]);
void GR0_dequeue(Queue* q, int value[2]);
void GR0_displayQueue(Queue* q);
void GR0_initQueue(Queue* q);

void GR0_initQueues(Queue* q);
void GR0_resetQueue(Queue* q);

void GR0_resetQueues(Queue* q) ;
void GR0_freeQueues(Queue* q);
void GR0_displayQueue(Queue* q);
void GR0_resizeQueue(Queue* q);
void GR0_freeQueue(Queue* q); 
int GR0_isinQueue(Queue* q, int value[2]);
void GR0_insert_sorted(Queue* q, int value[2]);

void GR0_concatenateQueue(Queue* queue1, Queue* queue2);
void GR0_freeQueue(Queue* q);
Queue* GR0_copyQueue(Queue* original);
void GR0_testQueue() ;

void GR0_displayQueues(Queue* q);

#endif