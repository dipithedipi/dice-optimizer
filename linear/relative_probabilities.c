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

    for (int i = 0; i < COMPARISON; i++) {
        int rollA = a->combination[randInt(0, NUM_ELEMENTS - 1)];
        int rollB = b->combination[randInt(0, NUM_ELEMENTS - 1)];

        if (rollA > rollB) {
            win++;
        } else if (rollA < rollB) {
            lose++;
        } else {
            tie++;
        }
    }

    float winProb = (float)win / COMPARISON * 100;
    float loseProb = (float)lose / COMPARISON * 100;
    float tieProb = (float)tie / COMPARISON * 100;

    fprintf(file, "Dice A: [");
    for (int i = 0; i < NUM_ELEMENTS; i++)
        fprintf(file, "%d%s", a->combination[i], (i == NUM_ELEMENTS - 1) ? "]\n" : ",");
    fprintf(file, "Dice B: [");
    for (int i = 0; i < NUM_ELEMENTS; i++)
        fprintf(file, "%d%s", b->combination[i], (i == NUM_ELEMENTS - 1) ? "]\n" : ",");
    fprintf(file, "Wins: %.2f%% | Losses: %.2f%% | Ties: %.2f%%\n\n", winProb, loseProb, tieProb);
}

int main() {
    srand(time(NULL));

    FILE *outputFile;
    outputFile = fopen("dice_probabilities.txt", "w");
    if (outputFile == NULL) {
        printf("Errore nell'apertura del file.");
        return 1;
    }

    // Allocazione dinamica per memorizzare le combinazioni trovate
    int maxCombinations = 5000;
    Combination *resultsA = (Combination *)malloc(maxCombinations * sizeof(Combination));
    Combination *resultsB = (Combination *)malloc(maxCombinations * sizeof(Combination));
    int countA = 0, countB = 0;

    int arr[NUM_ELEMENTS];
    findCombinations(arr, 0, TARGET_SUM, resultsA, &countA);
    findCombinations(arr, 0, TARGET_SUM, resultsB, &countB);

    // Troviamo e salviamo le informazioni su tutte le combinazioni
    for (int i = 0; i < countA; i++) {
        for (int j = 0; j < countB; j++) {
            evaluateDice(&resultsA[i], &resultsB[j], outputFile);
        }
    }

    // Liberiamo la memoria allocata
    free(resultsA);
    free(resultsB);

    // Chiudiamo il file
    fclose(outputFile);

    return 0;
}
