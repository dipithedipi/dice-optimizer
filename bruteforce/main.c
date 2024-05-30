#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TRIES 6000000
#define COMPARISON 7500
#define ARRAY_SIZE 6
#define MAX 30
#define MIN 0
#define MAX_SUM 30

enum { diceA, diceB };

int randInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

void generateDice(int arr[], int n) {
    int sum = 0;
    for (int i = 0; i < n-1; i++) {
        arr[i] = randInt(MIN, MAX - sum - (n - 1 - i));
        sum += arr[i];
    }
    arr[n-1] = MAX_SUM - sum;
}

void printDice(int arr[], int n) {
    printf("Dice: [");
    for (int i = 0; i < n; i++) {
        if (i != 0) printf(", ");
        printf("%d", arr[i]);
    }
    printf("]\n");
}

void saveData(FILE *file, int arr[], int wins) {
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    fprintf(file, "%d|", wins);
    for (int i = 0; i < ARRAY_SIZE-1; i++) {
        fprintf(file, "%d-", arr[i]);
    }
    fprintf(file, "%d\n", arr[ARRAY_SIZE-1]);
}

void bruteForce(FILE *file, int startDiceA[], int startDiceB[], int comparison, int tries) {
    int indexA, indexB;
    int winA, winB;

    int bestWinStrike = 0; // wins in a row for the best dice compared to the other
    int winner = diceA;

    for (int _ = 0; _ < tries; _++) {
        winA = 0;
        winB = 0;

        for (int i = 0; i < comparison; i++) {
            indexA = randInt(0, ARRAY_SIZE-1);
            indexB = randInt(0, ARRAY_SIZE-1);

            if (startDiceA[indexA] > startDiceB[indexB]) {
                winA++;
            } else if (startDiceA[indexA] < startDiceB[indexB]) {
                winB++;
            }
        }

        if (winA > winB) {
            if (winner == diceA) {
                bestWinStrike++;
            } else {
                saveData(file, startDiceB, bestWinStrike);
                bestWinStrike = 1;
                winner = diceA;
            }
            generateDice(startDiceB, ARRAY_SIZE);
        } else if (winB > winA) {
           if (winner == diceB) {
                bestWinStrike++;
            } else {
                saveData(file, startDiceA, bestWinStrike);
                bestWinStrike = 1;
                winner = diceB;
            }
            generateDice(startDiceA, ARRAY_SIZE);
        }
    }
}

int main() {
    FILE *file = fopen("data.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing\n");
        return 1;
    }
    srand(time(NULL));

    int diceA[ARRAY_SIZE];
    int diceB[ARRAY_SIZE];

    generateDice(diceA, ARRAY_SIZE);
    generateDice(diceB, ARRAY_SIZE);

    printf("Start brute force\n");
    bruteForce(file, diceA, diceB, COMPARISON, TRIES);

    printf("Data saved to file\n");
    fclose(file);

    return 0;
}
