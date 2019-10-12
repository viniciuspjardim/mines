/* 
 * File:   viniciuslib.h
 * Author: Vinícius Jardim
 *
 * Created on 1 de Julho de 2011, 04:06
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Booleanos ->
#define FALSE 0
#define TRUE 1

// Limpar a tela no terminal ->
#define LIMPAR_TELA "\\e[H\\e[2J"

// Cores de font no terminal ->
#define COR_PADRAO "\033[0m"
#define PRETO "\033[0;30m"
#define VERMELHO "\033[0;31m"
#define VERDE "\033[0;32m"
#define MARROM "\033[0;33m"
#define AZUL "\033[0;34m"
#define ROXO "\033[0;35m"
#define CIANO "\033[0;36m"
#define CINZA_CLARO "\033[0;37m"
#define CINZA_ESCURO "\033[1;30m"
#define VERMELHO_CLARO "\033[1;31m"
#define VERDE_CLARO "\033[1;32m"
#define AMARELO "\033[1;33m"
#define AZUL_CLARO "\033[1;34m"
#define ROXO_CLARO "\033[1;35m"
#define CIANO_CLARO "\033[1;36m"
#define BRANCO "\033[1;37m"

#ifndef VINICIUSLIB_H
#define	VINICIUSLIB_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _1 {
        int lin;
        int col;
        int** mat;
    } MatrizInt;

    MatrizInt* novaMatrizInt(int lin, int col);
    int setMatrizInt(MatrizInt *m, int lin, int col, int val);
    void freeMatrizInt(MatrizInt* m);
    void imprimirMatrizInt(MatrizInt* m);

    int meuRand(int min, int max);
    int charEDigito(char c);
    int strEDigito(char* str);

#ifdef	__cplusplus
}
#endif

#endif	/* VINICIUSLIB_H */
