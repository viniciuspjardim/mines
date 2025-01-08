#include "mlib.h"

// Create a new integer matrix
IntMatrix* newIntMatrix(int rows, int cols) {
    IntMatrix* m = (IntMatrix*) malloc(sizeof(IntMatrix));
    if (m == NULL) {
        return NULL;
    }

    m->rows = rows;
    m->cols = cols;

    m->mat = (int**) malloc(rows * sizeof(int*));
    if (m->mat == NULL) {
        return NULL;
    }

    int i, j;

    for (i = 0; i < rows; i++) {
        m->mat[i] = (int*) malloc(cols * sizeof(int));
        if (m->mat[i] == NULL) {
            return NULL;
        }
        for (j = 0; j < cols; j++) {
            m->mat[i][j] = 0;
        }
    }

    return m;
}

// Set a value in the integer matrix
int setIntMatrix(IntMatrix* m, int row, int col, int val) {
    if (m == NULL) return FALSE;

    if (row >= m->rows || col >= m->cols || row < 0 || col < 0)
        return FALSE;
    else
        m->mat[row][col] = val;

    return TRUE;
}

// Free the integer matrix from memory
void freeIntMatrix(IntMatrix* m) {
    if (m == NULL) return;

    int i;

    for (i = 0; i < m->rows; i++) {
        free(m->mat[i]);
    }

    free(m->mat);
    free(m);
    m = NULL;
}

// Print the integer matrix
void printIntMatrix(IntMatrix* m) {
    if (m == NULL) return;

    int i, j;

    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            printf("%d; ", m->mat[i][j]);
        }
        printf("\n");
    }
}

// Other functions
int myRand(int min, int max) {
    if (min > max) {
        int temp = max;
        max = min;
        min = temp;
    }

    int interval = max - min + 1;

    return (rand() % interval) + min;
}

int charIsDigit(char c) {
    if (c < '0' || c > '9') return FALSE;

    return TRUE;
}

int strIsDigit(char* str) {
    int count = 0;

    while (str[count] != '\0' && str[count] != '\n') {
        if (charIsDigit(str[count]) == FALSE)
            return FALSE;
        count++;
    }

    if (count > 0) return TRUE;

    return FALSE;
}
