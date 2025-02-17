#include "../head/dictionnary.h"



int findInsertionIndex(Dico *entry, int val1, int val2) {
    int left = 0, right = entry->count - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (entry->values[mid * VALUE_SIZE] < val1 || 
            (entry->values[mid * VALUE_SIZE] == val1 && entry->values[mid * VALUE_SIZE + 1] < val2)) {
            left = mid + 1;
        } 
        else {
            right = mid - 1;
        }
    }
    return left;
}

void insert(int key, int val1, int val2) {
    int index = key - MIN_KEY;
    Dico *entry = &hashTable[index];

    if (entry->count == entry->capacity) {
        entry->capacity = (entry->capacity == 0) ? INITIAL_CAPACITY : (int)(entry->capacity * GROWTH_FACTOR);
        entry->values = realloc(entry->values, entry->capacity * VALUE_SIZE * sizeof(int));
    }

    int insertIndex = findInsertionIndex(entry, val1, val2);
    
    memmove(&entry->values[(insertIndex + 1) * VALUE_SIZE], &entry->values[insertIndex * VALUE_SIZE], 
            (entry->count - insertIndex) * VALUE_SIZE * sizeof(int));

    // Insert new value
    entry->values[insertIndex * VALUE_SIZE] = val1;
    entry->values[insertIndex * VALUE_SIZE + 1] = val2;
    entry->count++;
}

void search(int key) {
    int index = key - MIN_KEY;
    Dico *entry = &hashTable[index];

    if (entry->count == 0) {
        printf("Key %d not found!\n", key);
        return;
    }

    printf("Values for key %d:\n", key);
    for (int i = 0; i < entry->count; i++) {
        printf("[%d, %d]\n", entry->values[i * VALUE_SIZE], entry->values[i * VALUE_SIZE + 1]);
    }
}

int findValue(int key, int val1, int val2) {
    if (key < MIN_KEY || key > MAX_KEY) return 0;
    int index = key - MIN_KEY;
    Dico *entry = &hashTable[index];
    if (entry->count == 0) return 0;

    int left = 0, right = entry->count - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (entry->values[mid * VALUE_SIZE] == val1 && entry->values[mid * VALUE_SIZE + 1] == val2)
            return 1;
        else if (entry->values[mid * VALUE_SIZE] < val1 || 
                 (entry->values[mid * VALUE_SIZE] == val1 && entry->values[mid * VALUE_SIZE + 1] < val2))
            left = mid + 1;
        else
            right = mid - 1;
    }
    return 0;
}

void printDictionary() {
    printf("Dictionary Contents:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable[i].count > 0) {
            printf("Key %d:\n", i + MIN_KEY);
            for (int j = 0; j < hashTable[i].count; j++) {
                printf("  [%d, %d]\n", hashTable[i].values[j * VALUE_SIZE], hashTable[i].values[j * VALUE_SIZE + 1]);
            }
        }
    }
}




int main() {
    insert(3, 10, 20);
    insert(3, 30, 40);
    insert(5, 50, 60);
    insert(3, 70, 80);
    insert(7, 15, 25);
    insert(7, 35, 45);
    insert(7, 5, 15);

    printf("\nSearching key 3:\n");
    search(3);
    printf("\nSearching key 5:\n");
    search(5);
    printf("\nSearching key 7:\n");
    search(7);

    printf("\nChecking if [30, 40] exists in key 3: %s\n", findValue(3, 30, 40) ? "Found" : "Not Found");
    printf("Checking if [99, 99] exists in key 3: %s\n", findValue(3, 99, 99) ? "Found" : "Not Found");

    printf("\nFull dictionary:\n");
    printDictionary();

    printf("\nDeleting key 3\n");
    search(3);

    return 0;
}