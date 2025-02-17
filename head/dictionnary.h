#ifndef DICT_H
#define DICT_H

#include <stdio.h>
#include <stdlib.h>

#define MIN_KEY 3
#define MAX_KEY 9
#define TABLE_SIZE 7
#define VALUE_SIZE 2
#define INITIAL_CAPACITY 16
#define GROWTH_FACTOR 1.5

typedef struct {
    int *values;
    int count;
    int capacity;
} Dico;

Dico hashTable[TABLE_SIZE] = {0};

int findInsertionIndex(Dico *entry, int val1, int val2);

void insert(int key, int val1, int val2) ;

void search(int key);

int findValue(int key, int val1, int val2);

void printDictionary();

#endif