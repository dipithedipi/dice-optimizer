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

Combination diceToBeat = {{5,5,5,5,5,5}};

typedef struct {
    float winPercentage;
    float losePercentage;
    float tiePercentage;
} Result;

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

    int start = (index == 0)? MIN : arr[index - 1];

    for (int k = start; k <= MAX; k++) {
        arr[index] = k;
        findCombinations(arr, index + 1, reducedNum - k, results, count);
    }
}

void evaluateDice(Combination *a, Combination *b, float *winPercentage, float *losePercentage, float *tiePercentage) {
    int win = 0, lose = 0, tie = 0;

    for (int i = 0; i < COMPARISON; i++) {
        int rollA = a->combination[randInt(0, NUM_ELEMENTS - 1)]; // Genera un valore casuale per il dado A
        int rollB = diceToBeat.combination[0]; // Usa il valore fisso del dado da battere

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
    outputFile = fopen("better_to_beat.txt", "w");
    if (outputFile == NULL) {
        printf("Errore nell'apertura del file.");
        return 1;
    }

    // Allocazione dinamica per memorizzare le combinazioni trovate
    int maxCombinations = 5000;
    Combination *resultsA = (Combination *)malloc(maxCombinations * sizeof(Combination));
    Result *results = (Result *)malloc(maxCombinations * sizeof(Result));
    int countA = 0, countResults = 0;
    float maxWinPercentage = 0.0f;
    Combination bestDice;

    int arr[NUM_ELEMENTS];
    findCombinations(arr, 0, TARGET_SUM, resultsA, &countA);

    // Troviamo e salviamo le informazioni su tutte le combinazioni
    for (int i = 0; i < countA; i++) {
        float totalWinPercentage = 0.0f, totalLosePercentage = 0.0f, totalTiePercentage = 0.0f;
        for (int j = 0; j < COMPARISON; j++) { // Modificato per usare COMPARISON invece di countB
            float winPercentage, losePercentage, tiePercentage;
            evaluateDice(&resultsA[i], &diceToBeat, &winPercentage, &losePercentage, &tiePercentage);
            totalWinPercentage += winPercentage;
            totalLosePercentage += losePercentage;
            totalTiePercentage += tiePercentage;
        }
        totalWinPercentage /= COMPARISON; // Calcolo corretto della media
        totalLosePercentage /= COMPARISON; // Calcolo corretto della media
        totalTiePercentage /= COMPARISON; // Calcolo corretto della media

        if (totalWinPercentage > maxWinPercentage) {
            maxWinPercentage = totalWinPercentage;
            bestDice = resultsA[i];
        }

        fprintf(outputFile, "Dice A: [");
        for (int k = 0; k < NUM_ELEMENTS; k++)
            fprintf(outputFile, "%d%s", resultsA[i].combination[k], (k == NUM_ELEMENTS - 1)? "]\n" : ",");
        fprintf(outputFile, "Total win percentage: %.2f%%\n", totalWinPercentage);
        fprintf(outputFile, "Total lose percentage: %.2f%%\n", totalLosePercentage);
        fprintf(outputFile, "Total tie percentage: %.2f%%\n\n", totalTiePercentage);
    }

    fprintf(outputFile, "Best dice:\n");
    fprintf(outputFile, "Dice A: [");
    for (int k = 0; k < NUM_ELEMENTS; k++)
        fprintf(outputFile, "%d%s", bestDice.combination[k], (k == NUM_ELEMENTS - 1)? "]\n" : ",");

    // Liberiamo la memoria allocata
    free(resultsA);
    free(results);

    // Chiudiamo il file
    fclose(outputFile);

    return 0;
}
