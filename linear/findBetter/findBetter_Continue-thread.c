#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define COMPARISON 10000
#define TARGET_SUM 30
#define NUM_ELEMENTS 6
#define MIN 0
#define MAX 30
#define NUM_THREADS 4
#define MAX_SEARCH 100

typedef struct {
    int combination[NUM_ELEMENTS];
} Combination;

typedef struct {
    Combination *combinations;
    int start;
    int end;
    Combination *bestCombination;
    float *maxWinPercentage;
    pthread_mutex_t *mutex;
    Combination *diceToBeat;
} ThreadData;

int randInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

void findCombinations(int arr[], int index, int reducedNum, Combination *results, int *count) {
    if (reducedNum < 0) return;

    if (reducedNum == 0 && index == NUM_ELEMENTS) {
        for (int i = 0; i < NUM_ELEMENTS; i++)
            results[*count].combination[i] = arr[i];
        (*count)++;
        return;
    }

    if (index >= NUM_ELEMENTS) return;

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
        int rollB = b->combination[randInt(0, NUM_ELEMENTS - 1)];

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

void *threadFunction(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    float localMaxWinPercentage = 0.0f;
    Combination localBestCombination;

    for (int i = data->start; i < data->end; i++) {
        float winPercentage, losePercentage, tiePercentage;
        evaluateDice(&data->combinations[i], data->diceToBeat, &winPercentage, &losePercentage, &tiePercentage);

        if (winPercentage > localMaxWinPercentage) {
            localMaxWinPercentage = winPercentage;
            localBestCombination = data->combinations[i];
        }
    }

    pthread_mutex_lock(data->mutex);
    if (localMaxWinPercentage > *(data->maxWinPercentage)) {
        *(data->maxWinPercentage) = localMaxWinPercentage;
        *(data->bestCombination) = localBestCombination;
    }
    pthread_mutex_unlock(data->mutex);

    return NULL;
}

int main() {
    srand(time(NULL));

    FILE *outputFile;
    outputFile = fopen("better_of_better.txt", "w");
    if (outputFile == NULL) {
        printf("Errore nell'apertura del file.");
        return 1;
    }

    int maxCombinations = 5000;
    Combination *resultsA = (Combination *)malloc(maxCombinations * sizeof(Combination));
    int countA = 0;
    Combination diceToBeat = {{1,1,7,7,7,7}};

    int arr[NUM_ELEMENTS];
    findCombinations(arr, 0, TARGET_SUM, resultsA, &countA);

    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    for (int search = 0; search < MAX_SEARCH; search++) {
        float maxWinPercentage = 0.0f;
        Combination bestDice;

        int blockSize = countA / NUM_THREADS;

        for (int i = 0; i < NUM_THREADS; i++) {
            threadData[i].combinations = resultsA;
            threadData[i].start = i * blockSize;
            threadData[i].end = (i == NUM_THREADS - 1) ? countA : (i + 1) * blockSize;
            threadData[i].bestCombination = &bestDice;
            threadData[i].maxWinPercentage = &maxWinPercentage;
            threadData[i].mutex = &mutex;
            threadData[i].diceToBeat = &diceToBeat;

            pthread_create(&threads[i], NULL, threadFunction, &threadData[i]);
        }

        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        diceToBeat = bestDice;

        fprintf(outputFile, "Search %d:\n", search + 1);
        fprintf(outputFile, "Best dice: [");
        for (int k = 0; k < NUM_ELEMENTS; k++)
            fprintf(outputFile, "%d%s", bestDice.combination[k], (k == NUM_ELEMENTS - 1) ? "]\n" : ",");
        fprintf(outputFile, "Total win percentage: %.2f%%\n\n", maxWinPercentage);
    }

    free(resultsA);
    fclose(outputFile);

    return 0;
}
