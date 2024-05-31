#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COMPARISON 50000
#define NUM_ELEMENTS 6

typedef struct {
    int combination[NUM_ELEMENTS];
} Combination;

int randInt(int min, int max) {
    return min + rand() % (max - min + 1);
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

int main() {
    srand(time(NULL));

    Combination dice1 = {{5, 5, 5, 5, 5, 5}};
    Combination dice2 = {{0, 1, 7, 7, 7, 8}};
    float winPercentage= 0.0 , losePercentage= 0.0, tiePercentage= 0.0;

    evaluateDice(&dice1, &dice2, &winPercentage, &losePercentage, &tiePercentage);

    printf("Comparison between custom dice:\n");
    printf("Dice A: [");
    for (int k = 0; k < NUM_ELEMENTS; k++)
        printf("%d%s", dice1.combination[k], (k == NUM_ELEMENTS - 1) ? "]\n" : ",");
    printf("Dice B: [");
    for (int k = 0; k < NUM_ELEMENTS; k++)
        printf("%d%s", dice2.combination[k], (k == NUM_ELEMENTS - 1) ? "]\n" : ",");
    
    printf("\nStatistics Dice A:\n");
    printf("Win percentage: %.2f%%\n", winPercentage);
    printf("Lose percentage: %.2f%%\n", losePercentage);
    printf("Tie percentage: %.2f%%\n", tiePercentage);

    return 0;
}
