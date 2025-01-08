#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef MLIB_H
#define	MLIB_H

#define FALSE 0
#define TRUE 1

// Font colors in the terminal
#define DEFAULT_COLOR "\033[0m"
#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BROWN "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define LIGHT_GRAY "\033[0;37m"
#define DARK_GRAY "\033[1;30m"
#define LIGHT_RED "\033[1;31m"
#define LIGHT_GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define LIGHT_BLUE "\033[1;34m"
#define LIGHT_PURPLE "\033[1;35m"
#define LIGHT_CYAN "\033[1;36m"
#define WHITE "\033[1;37m"

// Structure for an integer matrix
typedef struct _1 {
    int rows;
    int cols;
    int** mat;
} IntMatrix;

// Function prototypes
IntMatrix* newIntMatrix(int rows, int cols);
int setIntMatrix(IntMatrix* m, int row, int col, int val);
void freeIntMatrix(IntMatrix* m);
void printIntMatrix(IntMatrix* m);

int myRand(int min, int max);
int charIsDigit(char c);
int strIsDigit(char* str);

#endif    /* MLIB_H */
