// SAVE ALL DICE NUMBER COMPARISON BETWEEN ALL DICES

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COMPARISON 7500
#define TARGET_SUM 30
#define NUM_ELEMENTS 6
#define MIN 0
#define MAX 30

// Definiamo una struttura per memorizzare le combinazioni trovate
typedef struct {
    int combination[NUM_ELEMENTS];
} Combination;

int randInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Funzione per trovare le combinazioni e salvarle nella matrice
void findCombinations(int arr[], int index, int reducedNum, Combination *results, int *count) {
    if (reducedNum < 0)
        return;

    if (reducedNum == 0 && index == NUM_ELEMENTS) {
        for (int i = 0; i < NUM_ELEMENTS; i++)
            results[*count].combination[i] = arr[i];
        (*count)++;
        return;
    }

    if (index >= NUM_ELEMENTS)
        return;

    int start = (index == 0) ? MIN : arr[index - 1];

    for (int k = start; k <= MAX; k++) {
        arr[index] = k;
        findCombinations(arr, index + 1, reducedNum - k, results, count);
    }
}

void evaluateDice(Combination *a, Combination *b, FILE *file) {
    int win = 0, lose = 0, tie = 0;
    int resultA[COMPARISON], resultB[COMPARISON];

    for (int i = 0; i < COMPARISON; i++) {
        int rollA = a->combination[randInt(0, NUM_ELEMENTS - 1)];
        int rollB = b->combination[randInt(0, NUM_ELEMENTS - 1)];

        if (rollA > rollB) {
            win++;
            resultA[i] = rollA;
            resultB[i] = rollB;
        } else if (rollA < rollB) {
            lose++;
            resultA[i] = rollA;
            resultB[i] = rollB;
        } else {
            tie++;
            resultA[i] = rollA;
            resultB[i] = rollB;
        }
    }

    fprintf(file, "Dice-A: [");
    for (int i = 0; i < NUM_ELEMENTS; i++)
        fprintf(file, "%d%s", a->combination[i], (i == NUM_ELEMENTS - 1) ? "]\t" : ",");

    fprintf(file, "Dice-B: [");
    for (int i = 0; i < NUM_ELEMENTS; i++)
        fprintf(file, "%d%s", b->combination[i], (i == NUM_ELEMENTS - 1) ? "]\t" : ",");

    fprintf(file, "| wins: %d ", win);
    fprintf(file, "| lose: %d ", lose);
    fprintf(file, "| tie: %d\n", tie);

    fprintf(file, "ResultA: [");
    for (int i = 0; i < COMPARISON; i++)
        fprintf(file, "%d%s", resultA[i], (i == COMPARISON - 1) ? "]\t" : ",");
    fprintf(file, "\n");

    fprintf(file, "ResultB: [");
    for (int i = 0; i < COMPARISON; i++)
        fprintf(file, "%d%s", resultB[i], (i == COMPARISON - 1) ? "]\t" : ",");
    fprintf(file, "\n\n");
}

int main() {
    srand(time(NULL));

    FILE *outputFile;
    outputFile = fopen("dice_results.txt", "w");
    if (outputFile == NULL) {
        printf("Errore nell'apertura del file.");
        return 1;
    }

    // Allocazione dinamica per memorizzare le combinazioni trovate
    int maxCombinations = 5000;
    Combination *results = (Combination *)malloc(maxCombinations * sizeof(Combination));
    int count = 0;

    int arr[NUM_ELEMENTS];
    findCombinations(arr, 0, TARGET_SUM, results, &count);

    // resize array
    results = (Combination *)realloc(results, count * sizeof(Combination));

    // Troviamo e salviamo le informazioni su tutte le combinazioni
    for (int i = 0; i < count; i++) {
        for (int j = i; j < count; j++) {
            evaluateDice(&results[i], &results[j], outputFile);
        }
    }

    // Liberiamo la memoria allocata
    free(results);

    // Chiudiamo il file
    fclose(outputFile);

    return 0;
}
