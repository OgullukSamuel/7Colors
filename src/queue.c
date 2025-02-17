#include <stdio.h>
#include "../head/queue.h"


int injection(int* values){
    return((values[0]+values[1])*(values[0]+values[1]+1)+values[1]);
}



// Initialisation de la queue
void initQueue(Queue* q) {
    q->front = 0;
    q->rear = -1;
    q->length = 0;
}

void insert_sorted(Queue* q, int (*value)) {
    int injected_value = injection(value);  // Obtenir l'injection des deux valeurs
    int left = 0; 
    int right = q->length;

    // Recherche binaire pour insérer l'élément dans la bonne position
    while (left < right) {
        int mid = (left + right) / 2;
        if (q->sorted_values[mid] < injected_value)
            left = mid + 1;
        else
            right = mid;
    }

    for (int i = q->length; i > left; i--) {
        q->sorted_values[i] = q->sorted_values[i - 1];
    }

    q->sorted_values[left] = injected_value;
}

void enqueue(Queue* q, int* value) {
    if (q->length == SIZE) return;

    q->rear = (q->rear + 1) % SIZE; 
    q->items[q->rear][0] = value[0];
    q->items[q->rear][1] = value[1];

    insert_sorted(q, value);
    q->length++;
}


void dequeue(Queue* q, int* value) {
    if (q->length == 0) {
        printf("Queue vide !\n");
        return;
    }

    value[0] = q->items[q->front][0];
    value[1] = q->items[q->front][1];

    q->front++;
    q->length--;

    int injected_value = injection(&value[0]);
    int left = 0;
    int right = q->length;

    while (left < right) {
        int mid = (left + right) / 2;
        if (q->sorted_values[mid] < injected_value)
            left = mid + 1;
        else
            right = mid;
    }

    for (int i = left; i < q->length; i++) {
        q->sorted_values[i] = q->sorted_values[i + 1];
    };
}

// Recherche dichotomique en O(log(n)) dans le tableau trié
int isinQueue(Queue* q, int value[2]) {
    if(q->length == 0) return 0;
    int injected_value = injection(value);
    int left = 0;
    int right = q->length - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        if (q->sorted_values[mid] == injected_value)
            return 1;  
        else if (q->sorted_values[mid] < injected_value)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return 0;
}

// Affichage de la queue
void displayQueue(Queue* q) {
    if (q->length == 0) {
        printf("Queue vide\n");
        return;
    }

    for (int i = q->front; i <= q->rear; i++) {
        printf("[%d, %d] -> ", q->items[i][0], q->items[i][1]);
    }
    printf("\n");
}