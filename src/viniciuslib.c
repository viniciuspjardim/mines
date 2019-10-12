/*
 * File:   viniciuslib.c
 * Author: Vinícius Jardim
 *
 * Created on 1 de Julho de 2011, 04:08
 */

#include "viniciuslib.h"

// Matriz ->
MatrizInt* novaMatrizInt(int lin, int col) {
    MatrizInt* m = (MatrizInt*) malloc(sizeof (MatrizInt));

    m->lin = lin;
    m->col = col;

    m->mat = (int**) malloc(lin * sizeof (int*));
    if (m->mat == NULL) {
        return NULL;
    }

    int i, j;
    for (i = 0; i < lin; i++) {
        m->mat[i] = (int*) malloc(col * sizeof (int));
        if (m->mat[i] == NULL) {
            return NULL;
        }
        for (j = 0; j < col; j++) {
            m->mat[i][j] = 0;
        }
    }
    return m;
}

int setMatrizInt(MatrizInt *m, int lin, int col, int val) {
    if (m == NULL) return FALSE;

    if (lin >= m->lin || col >= m->col || lin < 0 || col < 0)
        return FALSE;
    else
        m->mat[lin][col] = val;

    return TRUE;
}

void freeMatrizInt(MatrizInt* m) {
    if (m == NULL) return;

    int i;
    for (i = 0; i < m->lin; i++) {
        free(m->mat[i]);
    }
    free(m->mat);
    free(m);
    m = NULL;
}

void imprimirMatrizInt(MatrizInt* m) {
    if (m == NULL) return;

    int i, j;
    for (i = 0; i < m->lin; i++) {
        for (j = 0; j < m->col; j++) {
            printf("%d; ", m->mat[i][j]);
        }
        printf("\n");
    }
}
// <- Fim Matriz

// Outras funções ->
int meuRand(int min, int max) {
    if(min > max)
    {
        int aux = max;
        max = min;
        min = aux;
    }
    int intervalo = max - min +1;
    return (rand() % intervalo) + min;
}

int charEDigito(char c) {
    
    if(c != '0' && c != '1' && c != '2' && c != '3' && c != '4' &&
            c != '5' && c != '6' && c != '7' && c != '8' && c != '9')
        return FALSE;
    return TRUE;
}

int strEDigito(char* str) {
    int cont = 0;
    while(str[cont] != '\0' && str[cont] != '\n') {
        if(charEDigito(str[cont]) == FALSE)
            return FALSE;
        cont++;
    }
    if(cont > 0)
        return TRUE;
    return FALSE;
}
