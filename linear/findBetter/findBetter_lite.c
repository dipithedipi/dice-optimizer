#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COMPARISON 5000
#define TARGET_SUM 30
#define NUM_ELEMENTS 6
#define MIN 0
#define MAX 30

typedef struct {
    int combination[NUM_ELEMENTS];
} Combination;

Combination diceToBeat = {{7,7,1,1,7,7}};

typedef struct {
    float winPercentage;
    float losePercentage;
    float tiePercentage;
} Result;

int randInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

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

void evaluateDice(Combination *a, Combination *b, float *winPercentage, float *losePercentage, float *tiePercentage) {
    int win = 0, lose = 0, tie = 0;

    for (int i = 0; i < COMPARISON; i++) {
        int rollA = a->combination[randInt(0, NUM_ELEMENTS - 1)];
        int rollB = b->combination[0];

        if (rollA > rollB) {
            win++;
        } else if (rollA < rollB) {
            lose++;
        } else {
            tie++;
        }
    }

    *winPercentage = ((float)win / COMPARISON) * 100;
    *losePercentage = ((float)lose / COMPARISON) * 100;
    *tiePercentage = ((float)tie / COMPARISON) * 100;
}

int main() {
    srand(time(NULL));

    FILE *outputFile;
    outputFile = fopen("better_to_beat_lite.txt", "w");
    if (outputFile == NULL) {
        printf("Errore nell'apertura del file.");
        return 1;
    }

    int maxCombinations = 5000;
    Combination *resultsA = (Combination *)malloc(maxCombinations * sizeof(Combination));
    int countA = 0;
    float maxWinPercentage = 0.0f;
    Combination bestDice;

    int arr[NUM_ELEMENTS];
    findCombinations(arr, 0, TARGET_SUM, resultsA, &countA);

    for (int i = 0; i < countA; i++) {
        float winPercentage, losePercentage, tiePercentage;
        evaluateDice(&resultsA[i], &diceToBeat, &winPercentage, &losePercentage, &tiePercentage);

        if (winPercentage > maxWinPercentage) {
            maxWinPercentage = winPercentage;
            bestDice = resultsA[i];
        }
    }

    fprintf(outputFile, "Best dice:\n");
    fprintf(outputFile, "Dice A: [");
    for (int k = 0; k < NUM_ELEMENTS; k++)
        fprintf(outputFile, "%d%s", bestDice.combination[k], (k == NUM_ELEMENTS - 1) ? "]\n" : ",");

    fprintf(outputFile, "Total win percentage: %.2f%%\n", maxWinPercentage);

    free(resultsA);
    fclose(outputFile);

    return 0;
}
