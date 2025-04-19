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

int injection(int* values);
void enqueue(Queue* q, int value[2]);
void dequeue(Queue* q, int value[2]);
void displayQueue(Queue* q);
void initQueue(Queue* q);

void initQueues(Queue* q);
void resetQueue(Queue* q);

void resetQueues(Queue* q) ;
void freeQueues(Queue* q);
void displayQueue(Queue* q);
void GR0_resizeQueue(Queue* q);
void freeQueue(Queue* q); 
int isinQueue(Queue* q, int value[2]);
void insert_sorted(Queue* q, int value[2]);

void concatenateQueue(Queue* queue1, Queue* queue2);
void freeQueue(Queue* q);
Queue* copyQueue(Queue* original);
void testQueue() ;

#endif