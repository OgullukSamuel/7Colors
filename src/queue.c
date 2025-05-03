#include "../head/queue.h"


/*int injection(int* values){
    return((values[0]+values[1])*(values[0]+values[1]+1)+values[1]);
}*/



int injection(int* values) {
    return (values[0] << 16) | (values[1] & 0xFFFF);
}

void GR0_initQueue(Queue *q) {
    q->capacity = QUEUE_INIT_CAPACITY;
    q->length = 0;
    q->front = 0;
    q->rear = -1;
    q->items = malloc(q->capacity * sizeof(int) * 2); 
    for (int i = 0; i < q->capacity * 2; i++) {
        q->items[i] = -1;
    }
    q->sorted_values = malloc(q->capacity * sizeof(int)); 
    for (int i = 0; i < q->capacity; i++) {
        q->sorted_values[i] = -1; // Initialize to -1 or any other sentinel value
    }
    if (!q->items || !q->sorted_values) {
        printf("ERMALLOC dans initQueue\n");
        exit(1);
    }
}


void GR0_concatenateQueue(Queue* queue1, Queue* queue2){
    //on concatene la queue 2 dans la queue 1
    Queue queue3;
    GR0_copyQueue(queue2,&queue3);
    for(int i=0; i<queue3.length; i++){
        int value[2];
        GR0_dequeue(&queue3, value);
        GR0_enqueue(queue1, value);
    }
}

void GR0_copyQueue(Queue* original,Queue* copy) {
    GR0_initQueue(copy);

    copy->capacity = original->capacity;
    copy->length = original->length;
    copy->front = original->front;
    copy->rear = original->rear;

    copy->items = malloc(copy->capacity * sizeof(int) * 2);
    copy->sorted_values = malloc(copy->capacity * sizeof(int));
    if (!copy->items || !copy->sorted_values) {
        printf("ERMALLOC2 dans copyQueue\n");
        GR0_freeQueue(copy);
        exit(1);
    }

    for (int i = 0; i < copy->capacity * 2; i++) {
        copy->items[i] = original->items[i];
    }
    for (int i = 0; i < copy->capacity; i++) {
        copy->sorted_values[i] = original->sorted_values[i];
    }
    return;
}

void GR0_resizeQueue(Queue* q) {
    int old_capacity = q->capacity;
    int* new_items = malloc(2 * old_capacity * 2 * sizeof(int));
    int* new_sorted_values = malloc(2 * old_capacity * sizeof(int));
    if (!new_items || !new_sorted_values) {
        printf("ERMALLOC dans GR0_resizeQueue\n");
        exit(1);
    }

    for (int i = 0; i < q->length; i++) {
        int index = (q->front + i) % old_capacity;
        new_items[2 * i] = q->items[2 * index];
        new_items[2 * i + 1] = q->items[2 * index + 1];
    }

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


void GR0_freeQueue(Queue* q) {
    if (q->items) {
        free(q->items);
        q->items = NULL; 
    }
    if (q->sorted_values) {
        free(q->sorted_values);
        q->sorted_values = NULL;
    } 
    return;
}

void GR0_insert_sorted(Queue* q, int value[2]) {
    int injected_value = injection(value);
    int left = 0; 
    int right = q->length;
    while (left < right) {
        int mid = (left + right) / 2;
        if (q->sorted_values[mid] < injected_value)
            left = mid + 1;
        else
            right = mid;
    }
    if (q->length == q->capacity) {
        printf("ER overflow de sorted_values\n");
        exit(1);
    }

    for (int i = q->length; i > left; i--) {
        q->sorted_values[i] = q->sorted_values[i - 1];
    }

    q->sorted_values[left] = injected_value;
}

void GR0_enqueue(Queue* q, int value[2]) {
    if (q->length == q->capacity) {
        GR0_resizeQueue(q);
    }
    q->rear = (q->rear + 1) %  q->capacity;

    q->items[2*q->rear] = value[0];
    q->items[2*q->rear + 1] = value[1];

    GR0_insert_sorted(q, value);
    q->length++;
}

void GR0_dequeue(Queue* q, int value[2]) {
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

int GR0_isinQueue(Queue* q, int value[2]) {
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

void GR0_freeQueues(Queue* q) {
    for (int i = 0; i < 7; i++) {
        GR0_freeQueue(&q[i]);
    }
}

void GR0_initQueues(Queue* q){
    for (int i = 0; i < 7; i++) {
        GR0_initQueue(&q[i]);
    }
}

void GR0_resetQueue(Queue* q) {
    q->length = 0;
    q->front = 0;
    q->rear = -1;
    q->capacity = QUEUE_INIT_CAPACITY;

    q->items = realloc(q->items, q->capacity * sizeof(int) * 2);
    if (!q->items) {
        printf("ERREALLOC dans resetQueue\n");
        exit(1);
    }
    for (int i = 0; i < q->capacity * 2; i++) {
        q->items[i] = -1;
    }

    q->sorted_values = realloc(q->sorted_values, q->capacity * sizeof(int));
    if (!q->sorted_values) {
        printf("ERREALLOC2 dans resetQueue\n");
        exit(1);
    }
    for (int i = 0; i < q->capacity; i++) {
        q->sorted_values[i] = -1;
    }
}

void GR0_resetQueues(Queue* q) {
    for (int i = 0; i < 7; i++) {
        GR0_resetQueue(&q[i]);
    }
}

void GR0_displayQueue(Queue* q) {
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

void GR0_displayQueues(Queue* q){
    for (int i = 0; i < 7; i++) {
        printf("Queue %d: ", i);
        GR0_displayQueue(&q[i]);
    }
}


void GR0_testQueue() {
    Queue q;
    int value[2];

    printf("Début du test des files\n");

    printf("Test 1: initialisation\n");
    GR0_initQueue(&q);
    GR0_displayQueue(&q);


    printf("Test 2: ajout d'élement\n");
    for (int i = 0; i < 10; i++) {
        value[0] = i;
        value[1] = i * 2;
        GR0_enqueue(&q, value);
    }
    GR0_displayQueue(&q);


    printf("Test 3: enlever les éléments\n");
    printf("Queue length: %d\n", q.length);
    printf("Queue capacity: %d\n", q.capacity);

    for (int i = 0; i < 5; i++) {
        GR0_dequeue(&q, value);
        printf("enlevé: [%d, %d]\n", value[0], value[1]);
    }
    GR0_displayQueue(&q);
    printf("Queue length: %d\n", q.length);
    printf("Queue capacity: %d\n", q.capacity);


    printf("Test 4: verification que les éléments sont dans la file\n");
    value[0] = 5;
    value[1] = 10;
    printf("Is [%d, %d] in queue? %s\n", value[0], value[1], GR0_isinQueue(&q, value) ? "Yes" : "No");
    value[0] = 100;
    value[1] = 200;
    printf("Is [%d, %d] in queue? %s\n", value[0], value[1], GR0_isinQueue(&q, value) ? "Yes" : "No");


    printf("Test 6: ajout après avoir enlevé \n");
    for (int i = 0; i < 6; i++) {
        value[0] = i + 10;
        value[1] = (i + 10) * 2;
        GR0_enqueue(&q, value);
    }
    GR0_displayQueue(&q);
    printf("Queue length: %d\n", q.length);
    printf("Queue capacity: %d\n", q.capacity);


    printf("Test 5: Reset des files\n");
    GR0_resetQueue(&q);
    GR0_displayQueue(&q);


    printf("Test 6: ajout après reset\n");
    for (int i = 0; i < 6; i++) {
        value[0] = i + 10;
        value[1] = (i + 10) * 2;
        GR0_enqueue(&q, value);
    }
    GR0_displayQueue(&q);
    printf("Queue length: %d\n", q.length);
    printf("Queue capacity: %d\n", q.capacity);


    printf("Test 7: libérer les queue...\n");
    GR0_freeQueue(&q);
    printf("queue libérée avec succès.\n");


    printf("Test 8: initialisation de multiples queues\n");
    Queue queues[7];
    GR0_initQueues(queues);
    for (int i = 0; i < 7; i++) {
        printf("Queue %d:\n", i);
        GR0_displayQueue(&queues[i]);
    }


    printf("Test 9: reset des queues\n");
    GR0_resetQueues(queues);
    for (int i = 0; i < 7; i++) {
        printf("Queue %d après reset:\n", i);
        GR0_displayQueue(&queues[i]);
    }

    
    printf("Test 10: liberage de multiples queues\n");
    GR0_freeQueues(queues);
    printf("All queues freed successfully.\n");

    printf("=== Tous les tests ont eu du succès ===\n");
}