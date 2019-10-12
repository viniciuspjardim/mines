/*
 * File:   mines.c
 * Author: Vinícius Jardim
 *
 * Created on 2 de Julho de 2011, 23:09
 */

#include "viniciuslib.h"

// Constantes para o campo ->
#define BOMB -1
#define VAZIO 0

// Constantes para a tela ->
#define BANDEIRA -1
#define FECHADO 0
#define ABERTO 1 // trocar: abr 1 e fec 0;

// Constantes para o jogo ->
enum constJogo {
    INICIOJOGO,
    PERDEU,
    CONTINUA,
    GANHOU,
    NOVOJOGO,
    SAIR,
    ERRO
};

int iniciarCampo(MatrizInt* campo, int nBombs, int primeiroAberto);
void mudarBomba(MatrizInt* campo, int cLin, int cCol);
void preencherNumeros(MatrizInt* campo, int cLin, int cCol);
enum constJogo abrir(MatrizInt* campo, MatrizInt* tela, int abrir);
void abrirRec(MatrizInt* campo, MatrizInt* tela, int cLin, int cCol);
void marcar(MatrizInt* campo, MatrizInt* tela, int abrir);
enum constJogo verificarResultado(MatrizInt* campo, MatrizInt* tela);
void imprimirJogo(MatrizInt* campo, MatrizInt* tela);
void imprimirJogoWin(MatrizInt* campo, MatrizInt* tela);
void imprimirAjuda();
void revelarJogo(MatrizInt* tela);
enum constJogo interpretador(MatrizInt* campo, MatrizInt* tela, char* comando, int primeiraJogada, int* nBombs);
void desalocar(MatrizInt* campo, MatrizInt* tela, char* comando);
enum constJogo menu(int *lin, int *col, int *nBombs);
int contarBandeiras(MatrizInt* tela);


int iniciarCampo(MatrizInt* campo, int nBombs, int primeiroAberto) {
    if(nBombs > campo->lin * campo->col -10) return FALSE;

    int cLinAberto = primeiroAberto / campo->col;
    int cColAberto = primeiroAberto % campo->col;

    int i, j, nBombsAux = nBombs;
    for(i = 0; i < campo->lin; i++) {
        for(j = 0; j < campo->col; j++) {
            if(nBombsAux == 0) break;
            campo->mat[i][j] = BOMB;
            nBombsAux--;
        }
        if(nBombsAux == 0) break;
    }

    for(i = 0; i < campo->lin; i++)
    {
        for(j = 0; j < campo->col; j++) {

            int cLin = meuRand(0, campo->lin-1);
            int cCol = meuRand(0, campo->col-1);
            int aux = campo->mat[i][j];
            campo->mat[i][j] = campo->mat[cLin][cCol];
            campo->mat[cLin][cCol] = aux;
        }
    }

    
    mudarBomba(campo, cLinAberto, cColAberto);
    mudarBomba(campo, cLinAberto -1, cColAberto -1);
    mudarBomba(campo, cLinAberto -1, cColAberto);
    mudarBomba(campo, cLinAberto -1, cColAberto +1);
    mudarBomba(campo, cLinAberto, cColAberto +1);
    mudarBomba(campo, cLinAberto +1, cColAberto +1);
    mudarBomba(campo, cLinAberto +1, cColAberto);
    mudarBomba(campo, cLinAberto +1, cColAberto -1);
    mudarBomba(campo, cLinAberto, cColAberto -1);

    setMatrizInt(campo, cLinAberto, cColAberto, VAZIO);
    setMatrizInt(campo, cLinAberto -1, cColAberto -1, VAZIO);
    setMatrizInt(campo, cLinAberto -1, cColAberto, VAZIO);
    setMatrizInt(campo, cLinAberto -1, cColAberto +1, VAZIO);
    setMatrizInt(campo, cLinAberto, cColAberto +1, VAZIO);
    setMatrizInt(campo, cLinAberto +1, cColAberto +1, VAZIO);
    setMatrizInt(campo, cLinAberto +1, cColAberto, VAZIO);
    setMatrizInt(campo, cLinAberto +1, cColAberto -1, VAZIO);
    setMatrizInt(campo, cLinAberto, cColAberto -1, VAZIO);

    for(i = 0; i < campo->lin; i++)
    {
        for(j = 0; j < campo->col; j++) {

            if(campo->mat[i][j] == BOMB)
                preencherNumeros(campo, i, j);
        }
    }

    return TRUE;
}

void mudarBomba(MatrizInt* campo, int cLin, int cCol)
{
    if(cLin < 0 || cCol < 0) return;
    if(cLin >= campo->lin || cCol >= campo->col) return;
    if(campo->mat[cLin][cCol] != BOMB) return;

    int i, j, brk = FALSE;
    for (i = 0; i < campo->lin; i++) {
        for (j = 0; j < campo->col; j++) {
            if (campo->mat[i][j] == VAZIO) {
                campo->mat[i][j] = BOMB;
                brk = TRUE;
                break;
            }
        }
        if (brk) break;
    }
}

void preencherNumeros(MatrizInt* campo, int cLin, int cCol) {
    if (cLin - 1 >= 0 && cCol - 1 >= 0 && campo->mat[cLin - 1][cCol - 1] != BOMB)
        campo->mat[cLin - 1][cCol - 1]++;
    if (cLin - 1 >= 0 && campo->mat[cLin - 1][cCol] != BOMB)
        campo->mat[cLin - 1][cCol]++;
    if (cLin - 1 >= 0 && cCol + 1 < campo->col && campo->mat[cLin - 1][cCol + 1] != BOMB)
        campo->mat[cLin - 1][cCol + 1]++;
    if (cCol + 1 < campo->col && campo->mat[cLin][cCol + 1] != BOMB)
        campo->mat[cLin][cCol + 1]++;
    if (cLin + 1 < campo->lin && cCol + 1 < campo->col && campo->mat[cLin + 1][cCol + 1] != BOMB)
        campo->mat[cLin + 1][cCol + 1]++;
    if (cLin + 1 < campo->lin && campo->mat[cLin + 1][cCol] != BOMB)
        campo->mat[cLin + 1][cCol]++;
    if (cLin + 1 < campo->lin && cCol - 1 >= 0 && campo->mat[cLin + 1][cCol - 1] != BOMB)
        campo->mat[cLin + 1][cCol - 1]++;
    if (cCol - 1 >= 0 && campo->mat[cLin][cCol - 1] != BOMB)
        campo->mat[cLin][cCol - 1]++;
}

enum constJogo abrir(MatrizInt* campo, MatrizInt* tela, int abrir) {
    int cLinAbrir = abrir / campo->col;
    int cColAbrir = abrir % campo->col;
    if(tela->mat[cLinAbrir][cColAbrir] == BANDEIRA)
        return ERRO;
    if(tela->mat[cLinAbrir][cColAbrir] == ABERTO) return ERRO;
    
    if(campo->mat[cLinAbrir][cColAbrir] == BOMB) {
        return PERDEU;
    }
    else {
        abrirRec(campo, tela, cLinAbrir, cColAbrir);
    }
    return verificarResultado(campo, tela);
}

void abrirRec(MatrizInt* campo, MatrizInt* tela, int cLin, int cCol) {
    if(tela->mat[cLin][cCol] == ABERTO || campo->mat[cLin][cCol] == BOMB) return;
    tela->mat[cLin][cCol] = ABERTO;
    if(campo->mat[cLin][cCol] != VAZIO) return;

    if (cLin - 1 >= 0 && cCol - 1 >= 0)
        abrirRec(campo, tela, cLin -1, cCol -1);
    if (cLin - 1 >= 0)
        abrirRec(campo, tela, cLin -1, cCol);
    if (cLin - 1 >= 0 && cCol + 1 < campo->col)
        abrirRec(campo, tela, cLin -1, cCol +1);
    if (cCol + 1 < campo->col)
        abrirRec(campo, tela, cLin, cCol +1);
    if (cLin + 1 < campo->lin && cCol + 1 < campo->col)
        abrirRec(campo, tela, cLin +1, cCol +1);
    if (cLin + 1 < campo->lin)
        abrirRec(campo, tela, cLin +1, cCol);
    if (cLin + 1 < campo->lin && cCol - 1 >= 0)
        abrirRec(campo, tela, cLin +1, cCol -1);
    if (cCol - 1 >= 0)
        abrirRec(campo, tela, cLin, cCol -1);
}

void marcar(MatrizInt* campo, MatrizInt* tela, int abrir) {
    int cLinAbrir = abrir / campo->col;
    int cColAbrir = abrir % campo->col;
    if(tela->mat[cLinAbrir][cColAbrir] == ABERTO) return;

    if(tela->mat[cLinAbrir][cColAbrir] == FECHADO)
        tela->mat[cLinAbrir][cColAbrir] = BANDEIRA;
    else if(tela->mat[cLinAbrir][cColAbrir] == BANDEIRA)
        tela->mat[cLinAbrir][cColAbrir] = FECHADO;
}

enum constJogo verificarResultado(MatrizInt* campo, MatrizInt* tela) {
    int i, j;
    enum constJogo result = GANHOU;
    for(i = 0; i < campo->lin; i++) {
        for(j = 0; j < campo->col; j++) {
            if(tela->mat[i][j] == FECHADO && campo->mat[i][j] != BOMB) {
                result = CONTINUA;
                break;
            }
        }
        if(result == CONTINUA) break;
    }
    return result;
}

void imprimirJogo(MatrizInt* campo, MatrizInt* tela) {
    int i, j;

    system("cls");
    printf("====================================\n");
    printf(" CAMPO MINADO [por Vinicius Jardim]\n====================================\n\n");
    printf("      ");
    for(i = 0; i < campo->col; i++) {
        if(i < 10)
            printf(" %d ", i);
        else if(i < 100)
            printf("%d ", i);
    }
    printf("\n      ");
    for(i = 0; i < campo->col; i++) {
        printf("---");
    }
    printf("\n");

    for(i = 0; i < campo->lin; i++) {
        printf("%5d|", i * campo->col);
        for(j = 0; j < campo->col; j++) {
            if(tela->mat[i][j] == BANDEIRA)
                printf(" P ");
            else if(tela->mat[i][j] == FECHADO)
                printf(CINZA_ESCURO "[ ]" COR_PADRAO);
            else if(tela->mat[i][j] == ABERTO && campo->mat[i][j] == BOMB)
                printf(" x ");
            else if(tela->mat[i][j] == ABERTO && campo->mat[i][j] == VAZIO)
                printf(CINZA_ESCURO " . " COR_PADRAO);
            else if(tela->mat[i][j] == ABERTO) {
                int val = campo->mat[i][j];

                if(val == 1)
                    printf(VERDE);
                else if(val == 2)
                    printf(MARROM);
                else if(val == 3)
                    printf(VERMELHO);
                else if(val == 4)
                    printf(ROXO);
                else if(val == 5)
                    printf(AZUL);
                else if(val == 6)
                    printf(CIANO);
                else if(val == 7)
                    printf(AMARELO);
                else if(val == 8)
                    printf(AZUL_CLARO);

                printf(" %d ", val);
                printf(COR_PADRAO);
            }
        }
        printf("\n");
    }
}

void imprimirJogoWin(MatrizInt* campo, MatrizInt* tela) {
    int i, j;

    system("cls");
    printf("====================================\n");
    printf(" CAMPO MINADO [por Vinicius Jardim]\n====================================\n\n");
    printf("      ");
    for(i = 0; i < campo->col; i++) {
        if(i < 10)
            printf(" %d ", i);
        else if(i < 100)
            printf("%d ", i);
    }
    printf("\n      ");
    for(i = 0; i < campo->col; i++) {
        printf("---");
    }
    printf("\n");

    for(i = 0; i < campo->lin; i++) {
        printf("%5d|", i * campo->col);
        for(j = 0; j < campo->col; j++) {
            if(tela->mat[i][j] == BANDEIRA)
                printf(" P ");
            else if(tela->mat[i][j] == FECHADO)
                printf("[ ]");
            else if(tela->mat[i][j] == ABERTO && campo->mat[i][j] == BOMB)
                printf(" x ");
            else if(tela->mat[i][j] == ABERTO && campo->mat[i][j] == VAZIO)
                printf(" . ");
            else if(tela->mat[i][j] == ABERTO) {
                int val = campo->mat[i][j];
                printf(" %d ", val);
            }
        }
        printf("\n");
    }
}

void imprimirAjuda() {

    system("cls");
    printf("====================================\n");
    printf(" CAMPO MINADO [por Vinicius Jardim]\n====================================\n\n");

    printf("Instrucoes:\n");
    printf("   Jogada  -  Para revelar digite o numero do quadrado " CINZA_ESCURO "[ ]" COR_PADRAO " desejado. Este\n");
    printf("              numero sera a soma do numero da linha com o numero da coluna.\n");
    printf("   Marcar  -  Para marcar uma mina, digite um asterisco " CINZA_ESCURO "*" COR_PADRAO " e faca o mesmo\n");
    printf("              procedimento da jogada para ecnontrar o numero do quadrado.\n\n");

    printf("Comandos:\n");
    printf("   -n         Comeca uma nova partida. Pode ser digitado no final ou durante\n");
    printf("              a partida. Caso seja digitado durante a partida, ecerra a mesma.\n");
    printf("   -s         Sai do jogo, retornando para a linha de comando.\n\n");

    printf("Execucao: (o jogo podera ser iniciado com os seguintes parametros)\n");
    printf("   -lin num   Numero de linhas.\n");
    printf("   -col num   Numero de colunas.\n");
    printf("   -bomb num  Numero de bombas.\n\n");

    printf("Versao: 0.10; contato: viniciusjardim@uft.edu.br; julho de 2011.\n");
    printf("Pressione " CINZA_ESCURO "r" COR_PADRAO " para retornar: ");
    char* c = (char*)malloc(sizeof(char));
    scanf("%s", c);
    free(c);
}

void revelarJogo(MatrizInt* tela) {
    int i, j;
    for(i = 0; i < tela->lin; i++) {
        for(j = 0; j < tela->col; j++) {
            tela->mat[i][j] = ABERTO;
        }
    }
}

enum constJogo interpretador(MatrizInt* campo, MatrizInt* tela, char* comando, int primeiraJogada, int* nBombs) {
    char subStr[10];
    int abrirN;
    if(comando[0] == '-') {
        if(comando[1] == 'n')
            return NOVOJOGO;
        else if(comando[1] == 's')
            return SAIR;
        else if(comando[1] == 'a') {
            imprimirAjuda();
            return ERRO;
        }
    }
    else if(strcmp(comando, "> fwin\n") == 0) {
        return GANHOU;
    }
    else if(comando[0] == '*') {
        strncpy(subStr, comando+1, 9);
        if(strEDigito(subStr)) {
            abrirN = atoi(subStr);
            if(abrirN < campo->lin * campo->col && abrirN >= 0)
                marcar(campo, tela, abrirN);
            return ERRO;
        }
    }
    else if(strEDigito(comando)) {
        abrirN = atoi(comando);
        if(abrirN < campo->lin * campo->col && abrirN >= 0) {
            if(primeiraJogada)
                iniciarCampo(campo, *nBombs, abrirN);
            return abrir(campo, tela, abrirN);
        }
    }
    return ERRO;
}

void desalocar(MatrizInt* campo, MatrizInt* tela, char* comando) {
    freeMatrizInt(campo);
    campo = NULL;
    freeMatrizInt(tela);
    tela = NULL;
    free(comando);
    comando = NULL;
}

int contarBandeiras(MatrizInt* tela) {
    int cont = 0;
    int i;
    for(i = 0; i < tela->lin; i++) {
        int j;
        for(j = 0; j < tela->col; j++) {
            if(tela->mat[i][j] == BANDEIRA) cont++;
        }
    }
    return cont;
}

enum constJogo menu(int *lin, int *col, int *nBombs) {
    MatrizInt* campo = novaMatrizInt(*lin, *col);
    MatrizInt* tela = novaMatrizInt(*lin, *col);

    int abrirN;
    enum constJogo result;
    char* comando = (char*)malloc(10* sizeof(char));

    int primeiraJogada = TRUE;
    time_t tempo;

    while (TRUE) {
        int nBandeiras = contarBandeiras(tela);
        imprimirJogoWin(campo, tela);
        printf("\n %d/%d bombas  |  -a para ajuda.\n====================================", nBandeiras, *nBombs);
        printf("\n Jogar: ");
        fgets(comando, 9, stdin);
        result = interpretador(campo, tela, comando, primeiraJogada, nBombs);
        if(result == CONTINUA && primeiraJogada) {
            primeiraJogada = FALSE;
            tempo = time(NULL);
        }

        if (result == NOVOJOGO || result == SAIR) {
            desalocar(campo, tela, comando);
            return result;
        } else if (result != CONTINUA && result != ERRO) break;
    }
    revelarJogo(tela);
    imprimirJogoWin(campo, tela);
    if (result == PERDEU) {
        printf(VERMELHO);
        printf("\n PERDEU!");
    } else if (result == GANHOU) {
        printf(VERDE);
        printf("\n GANHOU!");
    }
    printf(COR_PADRAO);

    tempo = time(NULL) -tempo;

    printf(" Tempo: %dm e %ds\n====================================\n", ((int)tempo)/60, ((int)tempo)%60);

    printf(" -n = novo jogo; -s = sair: ");
    scanf("%s", comando);

    result = interpretador(campo, tela, comando, FALSE, nBombs);
    desalocar(campo, tela, comando);
    return result;
}

int main(int argc, char** argv) {
    srand(time(NULL));
    
    int lin = 10, col = 10, nBombs = 10;
    int i, erro = FALSE;

    for(i = 1; i < argc; i++) {
        if(erro) break;

        if(strcmp(argv[i], "-lin") == 0 && i +1 < argc) {
            if(strEDigito(argv[i+1]))
                lin = atoi(argv[i+1]);
            else erro = TRUE;
        }
        else if(strcmp(argv[i], "-col") == 0 && i +1 < argc) {
            if(strEDigito(argv[i+1]))
                col = atoi(argv[i+1]);
            else erro = TRUE;
        }
        else if(strcmp(argv[i], "-bomb") == 0 && i +1 < argc) {
            if(strEDigito(argv[i+1]))
                nBombs = atoi(argv[i+1]);
            else erro = TRUE;
        }
    }
    if(erro == FALSE) {
        if(nBombs > lin * col -10)
            nBombs = lin * col -10;
        while(menu(&lin, &col, &nBombs) == NOVOJOGO){}
    }
    else {
        printf("Parametros invalidos, use os da seguinte forma:\n\t-lin numeroDeLinhas\n\t");
        printf("-col numeroDeColunas\n\t-bomb numeroDeBombas\n");
    }

    return (EXIT_SUCCESS);
}