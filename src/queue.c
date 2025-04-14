#include "../head/queue.h"

int injection(int* values){
    return((values[0]+values[1])*(values[0]+values[1]+1)+values[1]);
}

/*
int injection(int a, int b) {
    return a * 73856093 ^ b * 19349663;
}*/

/*
int injection(int* values) {
    return (values[0] << 16) | (values[1] & 0xFFFF);
}*/

void GR0_resizeQueue(Queue* q) {
    int old_capacity = q->capacity;
    int* new_items = malloc(2 * old_capacity * 2 * sizeof(int)); // double capacité (chaque élément = 2 int)
    int* new_sorted_values = malloc(2 * old_capacity * sizeof(int));
    if (!new_items || !new_sorted_values) {
        printf("[ERROR] Memory allocation failed in GR0_resizeQueue\n");
        exit(1);
    }

    // Réaligner les éléments dans le nouvel array
    for (int i = 0; i < q->length; i++) {
        int index = (q->front + i) % old_capacity;
        new_items[2 * i] = q->items[2 * index];
        new_items[2 * i + 1] = q->items[2 * index + 1];
    }

    // Copier sorted_values
    for (int i = 0; i < q->length; i++) {
        new_sorted_values[i] = q->sorted_values[i];
    }
    for (int i = q->length; i < 2 * old_capacity; i++) {
        new_sorted_values[i] = -1;
    }

    free(q->items);
    free(q->sorted_values);

    q->items = new_items;
    q->sorted_values = new_sorted_values;

    q->front = 0;
    q->rear = q->length - 1;
    q->capacity = 2 * old_capacity;
}

void initQueue(Queue *q) {
    q->capacity = QUEUE_INIT_CAPACITY;
    q->length = 0;
    q->front = 0;
    q->rear = -1;
    q->items = malloc(q->capacity * sizeof(int) * 2); 
    for (int i = 0; i < q->capacity * 2; i++) {
        q->items[i] = -1; // Initialize to -1 or any other sentinel value
    }
    q->sorted_values = malloc(q->capacity * sizeof(int)); 
    for (int i = 0; i < q->capacity; i++) {
        q->sorted_values[i] = -1; // Initialize to -1 or any other sentinel value
    }
    if (!q->items || !q->sorted_values) {
        printf("[ERROR] Memory allocation failed in initQueue\n");
        exit(1);
    }
}

void freeQueue(Queue* q) {
    if (q->items) {
        free(q->items);
        q->items = NULL; // Prevent double free
    }
    if (q->sorted_values) {
        free(q->sorted_values);
        q->sorted_values = NULL; // Prevent double free
    }
}

void insert_sorted(Queue* q, int value[2]) {
    int injected_value = injection(value);
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

    // Vérifier si la capacité est suffisante
    if (q->length == q->capacity) {
        printf("[ERROR] Not enough capacity in sorted_values array\n");
        exit(1);
    }

    // Décaler les éléments pour insérer la nouvelle valeur
    for (int i = q->length; i > left; i--) {
        q->sorted_values[i] = q->sorted_values[i - 1];
    }

    q->sorted_values[left] = injected_value;
}

void enqueue(Queue* q, int value[2]) {
    if (q->length == q->capacity) {
        GR0_resizeQueue(q);
    }
    q->rear = (q->rear + 1) %  q->capacity;

    q->items[2*q->rear] = value[0];
    q->items[2*q->rear + 1] = value[1];

    insert_sorted(q, value);
    q->length++;
}

void dequeue(Queue* q, int value[2]) {
    if (q->length == 0) {
        printf("Queue vide !\n");
        return;
    }

    value[0] = q->items[2*q->front];
    value[1] = q->items[2*q->front+1];

    q->front = (q->front + 1) % q->capacity;
    int right = q->length;
    q->length--;

    int injected_value = injection(value);
    int left = 0;


    while (left < right) {
        int mid = (left + right) / 2;
        if (q->sorted_values[mid] < injected_value)
            left = mid + 1;
        else
            right = mid;
    }

    for (int i = left; i < q->length; i++) {
        q->sorted_values[i] = q->sorted_values[i + 1];
    }
}

// Recherche dichotomique en O(log(n)) dans le tableau trié
int isinQueue(Queue* q, int value[2]) {
    if (q->length == 0) return 0;
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

void freeQueues(Queue* q){
    for (int i = 0; i < 7; i++) {
        freeQueue(&q[i]);
    }
}

void initQueues(Queue* q){
    for (int i = 0; i < 7; i++) {
        initQueue(&q[i]);
    }
}
void resetQueue(Queue* q) {
    q->length = 0;
    q->front = 0;
    q->rear = -1;
    q->capacity = QUEUE_INIT_CAPACITY;

    // Reallocate items array
    q->items = realloc(q->items, q->capacity * sizeof(int) * 2);
    if (!q->items) {
        printf("[ERROR] Memory reallocation failed for items in resetQueue\n");
        exit(1);
    }
    for (int i = 0; i < q->capacity * 2; i++) {
        q->items[i] = -1;
    }

    // Reallocate sorted_values array
    q->sorted_values = realloc(q->sorted_values, q->capacity * sizeof(int));
    if (!q->sorted_values) {
        printf("[ERROR] Memory reallocation failed for sorted_values in resetQueue\n");
        exit(1);
    }
    for (int i = 0; i < q->capacity; i++) {
        q->sorted_values[i] = -1;
    }
}

void resetQueues(Queue* q) {
    for (int i = 0; i < 7; i++) {
        resetQueue(&q[i]);
    }
}

void displayQueue(Queue* q) {
    if (q->length == 0) {
        printf("Queue vide !\n");
        return;
    }
    printf("Queue : ");
    for (int i = 0; i < q->length; i++) {
        int index = (q->front + i) % q->capacity;
        printf("[%d, %d] ", q->items[2*index], q->items[2*index+1]);
    }
    printf("\n");
}


void testQueue() {
    Queue q;
    int value[2];

    printf("=== Testing Queue Implementation ===\n");

    // Test 1: Initialize the queue
    printf("Test 1: Initializing queue...\n");
    initQueue(&q);
    displayQueue(&q);

    // Test 2: Enqueue elements
    printf("Test 2: Enqueueing elements...\n");
    for (int i = 0; i < 10; i++) {
        value[0] = i;
        value[1] = i * 2;
        enqueue(&q, value);
    }
    displayQueue(&q);

    // Test 3: Dequeue elements
    printf("Test 3: Dequeueing elements...\n");
    printf("Queue length: %d\n", q.length);
    printf("Queue capacity: %d\n", q.capacity);

    for (int i = 0; i < 5; i++) {
        dequeue(&q, value);
        printf("Dequeued: [%d, %d]\n", value[0], value[1]);
    }
    displayQueue(&q);
    printf("Queue length: %d\n", q.length);
    printf("Queue capacity: %d\n", q.capacity);


    // Test 4: Check if elements are in the queue
    printf("Test 4: Checking if elements are in the queue...\n");
    value[0] = 5;
    value[1] = 10;
    printf("Is [%d, %d] in queue? %s\n", value[0], value[1], isinQueue(&q, value) ? "Yes" : "No");
    value[0] = 100;
    value[1] = 200;
    printf("Is [%d, %d] in queue? %s\n", value[0], value[1], isinQueue(&q, value) ? "Yes" : "No");

    printf("Test 6: Enqueueing after dequeue...\n");
    for (int i = 0; i < 6; i++) {
        value[0] = i + 10;
        value[1] = (i + 10) * 2;
        enqueue(&q, value);
    }
    displayQueue(&q);
    printf("Queue length: %d\n", q.length);
    printf("Queue capacity: %d\n", q.capacity);


    // Test 5: Reset the queue
    printf("Test 5: Resetting the queue...\n");
    resetQueue(&q);
    displayQueue(&q);

    // Test 6: Enqueue after reset
    printf("Test 6: Enqueueing after reset...\n");
    for (int i = 0; i < 6; i++) {
        value[0] = i + 10;
        value[1] = (i + 10) * 2;
        enqueue(&q, value);
    }
    displayQueue(&q);
    printf("Queue length: %d\n", q.length);
    printf("Queue capacity: %d\n", q.capacity);

    // Test 7: Free the queue
    printf("Test 7: Freeing the queue...\n");
    freeQueue(&q);
    printf("Queue freed successfully.\n");

    // Test 8: Initialize multiple queues
    printf("Test 8: Initializing multiple queues...\n");
    Queue queues[7];
    initQueues(queues);
    for (int i = 0; i < 7; i++) {
        printf("Queue %d:\n", i);
        displayQueue(&queues[i]);
    }

    // Test 9: Reset multiple queues
    printf("Test 9: Resetting multiple queues...\n");
    resetQueues(queues);
    for (int i = 0; i < 7; i++) {
        printf("Queue %d after reset:\n", i);
        displayQueue(&queues[i]);
    }

    // Test 10: Free multiple queues
    printf("Test 10: Freeing multiple queues...\n");
    freeQueues(queues);
    printf("All queues freed successfully.\n");

    printf("=== All Tests Completed ===\n");
}