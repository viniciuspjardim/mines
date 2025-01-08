#include "mlib.h"

// Constants for the minefield
#define BOMB -1
#define EMPTY 0

// Constants for the screen
#define FLAG -1
#define CLOSED 0
#define OPENED 1

// Constants for the game
enum gameConsts {
    STARTGAME,
    LOST,
    KEEPGOING,
    WON,
    NEWGAME,
    EXIT,
    ERROR
};

// Function prototypes related to the Minesweeper game
int initField(IntMatrix* field, int bombCount, int firstOpen);
void changeBomb(IntMatrix* field, int row, int col);
void fillNumbers(IntMatrix* field, int row, int col);
enum gameConsts openCell(IntMatrix* field, IntMatrix* screen, int openPos);
void openRec(IntMatrix* field, IntMatrix* screen, int row, int col);
void markCell(IntMatrix* field, IntMatrix* screen, int openPos);
enum gameConsts checkResult(IntMatrix* field, IntMatrix* screen);
void printGame(IntMatrix* field, IntMatrix* screen);
void printHelp();
void revealGame(IntMatrix* screen);
enum gameConsts interpreter(IntMatrix* field, IntMatrix* screen, char* command, int firstMove, int* bombCount);
void deallocate(IntMatrix* field, IntMatrix* screen, char* command);
enum gameConsts menuGame(int *rows, int *cols, int *bombCount);
int countFlags(IntMatrix* screen);

// Initialize the minefield
int initField(IntMatrix* field, int bombCount, int firstOpen) {
    // Ensure we don't place too many bombs
    if (bombCount > field->rows * field->cols - 10) return FALSE;

    int openRow = firstOpen / field->cols;
    int openCol = firstOpen % field->cols;

    int i, j, bombCountAux = bombCount;

    // Place bombs in the top-left positions until we reach bombCount
    for (i = 0; i < field->rows; i++) {
        for (j = 0; j < field->cols; j++) {
            if (bombCountAux == 0) break;
            field->mat[i][j] = BOMB;
            bombCountAux--;
        }
        if (bombCountAux == 0) break;
    }

    // Shuffle bombs around the matrix
    for (i = 0; i < field->rows; i++) {
        for (j = 0; j < field->cols; j++) {
            int rRow = myRand(0, field->rows - 1);
            int rCol = myRand(0, field->cols - 1);
            int temp = field->mat[i][j];
            field->mat[i][j] = field->mat[rRow][rCol];
            field->mat[rRow][rCol] = temp;
        }
    }

    // Move bombs away from the position (and neighbors) of the first open
    changeBomb(field, openRow, openCol);
    changeBomb(field, openRow - 1, openCol - 1);
    changeBomb(field, openRow - 1, openCol);
    changeBomb(field, openRow - 1, openCol + 1);
    changeBomb(field, openRow, openCol + 1);
    changeBomb(field, openRow + 1, openCol + 1);
    changeBomb(field, openRow + 1, openCol);
    changeBomb(field, openRow + 1, openCol - 1);
    changeBomb(field, openRow, openCol - 1);

    // Force these positions to be EMPTY
    setIntMatrix(field, openRow, openCol, EMPTY);
    setIntMatrix(field, openRow - 1, openCol - 1, EMPTY);
    setIntMatrix(field, openRow - 1, openCol, EMPTY);
    setIntMatrix(field, openRow - 1, openCol + 1, EMPTY);
    setIntMatrix(field, openRow, openCol + 1, EMPTY);
    setIntMatrix(field, openRow + 1, openCol + 1, EMPTY);
    setIntMatrix(field, openRow + 1, openCol, EMPTY);
    setIntMatrix(field, openRow + 1, openCol - 1, EMPTY);
    setIntMatrix(field, openRow, openCol - 1, EMPTY);

    // Fill numeric hints around bombs
    for (i = 0; i < field->rows; i++) {
        for (j = 0; j < field->cols; j++) {
            if (field->mat[i][j] == BOMB)
                fillNumbers(field, i, j);
        }
    }

    return TRUE;
}

// Change bomb position if needed
void changeBomb(IntMatrix* field, int row, int col) {
    if (row < 0 || col < 0) return;
    if (row >= field->rows || col >= field->cols) return;
    if (field->mat[row][col] != BOMB) return;

    int i, j;

    for (i = 0; i < field->rows; i++) {
        for (j = 0; j < field->cols; j++) {
            if (field->mat[i][j] == EMPTY) {
                field->mat[i][j] = BOMB;
                return;
            }
        }
    }
}

// Increment numbers around bombs
void fillNumbers(IntMatrix* field, int row, int col) {
    if (row - 1 >= 0 && col - 1 >= 0 && field->mat[row - 1][col - 1] != BOMB)
        field->mat[row - 1][col - 1]++;
    if (row - 1 >= 0 && field->mat[row - 1][col] != BOMB)
        field->mat[row - 1][col]++;
    if (row - 1 >= 0 && col + 1 < field->cols && field->mat[row - 1][col + 1] != BOMB)
        field->mat[row - 1][col + 1]++;
    if (col + 1 < field->cols && field->mat[row][col + 1] != BOMB)
        field->mat[row][col + 1]++;
    if (row + 1 < field->rows && col + 1 < field->cols && field->mat[row + 1][col + 1] != BOMB)
        field->mat[row + 1][col + 1]++;
    if (row + 1 < field->rows && field->mat[row + 1][col] != BOMB)
        field->mat[row + 1][col]++;
    if (row + 1 < field->rows && col - 1 >= 0 && field->mat[row + 1][col - 1] != BOMB)
        field->mat[row + 1][col - 1]++;
    if (col - 1 >= 0 && field->mat[row][col - 1] != BOMB)
        field->mat[row][col - 1]++;
}

// Open a cell (reveal it)
enum gameConsts openCell(IntMatrix* field, IntMatrix* screen, int openPos) {
    int openRow = openPos / field->cols;
    int openCol = openPos % field->cols;

    if (screen->mat[openRow][openCol] == FLAG)
        return ERROR;

    if (screen->mat[openRow][openCol] == OPENED)
        return ERROR;

    if (field->mat[openRow][openCol] == BOMB) {
        return LOST;
    } else {
        openRec(field, screen, openRow, openCol);
    }

    return checkResult(field, screen);
}

// Recursive opening of empty areas
void openRec(IntMatrix* field, IntMatrix* screen, int row, int col) {
    // Return when out of matrix bounds
    if (row < 0 || row >= field->rows || col < 0 || col >= field->cols) return;

    // Return if it's already opened or a bomb
    if (screen->mat[row][col] == OPENED || field->mat[row][col] == BOMB) return;

    screen->mat[row][col] = OPENED;

    // Stop if the cell is not EMPTY
    if (field->mat[row][col] != EMPTY) return;

    // Recursively open all neighbors if it is EMPTY
    openRec(field, screen, row - 1, col - 1);
    openRec(field, screen, row - 1, col);
    openRec(field, screen, row - 1, col + 1);
    openRec(field, screen, row, col + 1);
    openRec(field, screen, row + 1, col + 1);
    openRec(field, screen, row + 1, col);
    openRec(field, screen, row + 1, col - 1);
    openRec(field, screen, row, col - 1);
}

// Mark (or unmark) a cell with a flag
void markCell(IntMatrix* field, IntMatrix* screen, int openPos) {
    int openRow = openPos / field->cols;
    int openCol = openPos % field->cols;
    if (screen->mat[openRow][openCol] == OPENED) return;

    if (screen->mat[openRow][openCol] == CLOSED)
        screen->mat[openRow][openCol] = FLAG;
    else if (screen->mat[openRow][openCol] == FLAG)
        screen->mat[openRow][openCol] = CLOSED;
}

// Check if the game is won or should continue
enum gameConsts checkResult(IntMatrix* field, IntMatrix* screen) {
    int i, j;

    for (i = 0; i < field->rows; i++) {
        for (j = 0; j < field->cols; j++) {
            // If there's still a CLOSED cell that isn't a bomb
            if (screen->mat[i][j] == CLOSED && field->mat[i][j] != BOMB) {
                return KEEPGOING;
            }
        }
    }

    return WON;
}

void printGame(IntMatrix* field, IntMatrix* screen) {
    int i, j;

    system("clear");
    printf("====================================\n");
    printf(" Minesweeper [by Vinicius Jardim]\n");
    printf("====================================\n\n");
    printf("      ");

    // Print the column indices
    for (i = 0; i < field->cols; i++) {
        if (i < 10)
            printf(" %d ", i);
        else if (i < 100)
            printf("%d ", i);
    }
    printf("\n      ");

    for (i = 0; i < field->cols; i++) {
        printf("---");
    }
    printf("\n");

    // Print each row
    for (i = 0; i < field->rows; i++) {
        printf("%5d|", i * field->cols);
        for (j = 0; j < field->cols; j++) {
            if (screen->mat[i][j] == FLAG)
                printf(" P ");
            else if (screen->mat[i][j] == CLOSED)
                printf(DARK_GRAY "[ ]" DEFAULT_COLOR);
            else if (screen->mat[i][j] == OPENED && field->mat[i][j] == BOMB)
                printf(" x ");
            else if (screen->mat[i][j] == OPENED && field->mat[i][j] == EMPTY)
                printf(DARK_GRAY " . " DEFAULT_COLOR);
            else if (screen->mat[i][j] == OPENED) {
                int val = field->mat[i][j];

                // Color for each number
                if (val == 1)        printf(GREEN);
                else if (val == 2)   printf(BROWN);
                else if (val == 3)   printf(RED);
                else if (val == 4)   printf(PURPLE);
                else if (val == 5)   printf(BLUE);
                else if (val == 6)   printf(CYAN);
                else if (val == 7)   printf(YELLOW);
                else if (val == 8)   printf(LIGHT_BLUE);

                printf(" %d ", val);
                printf(DEFAULT_COLOR);
            }
        }
        printf("\n");
    }
}

void printHelp() {
    system("clear");
    printf("====================================\n");
    printf(" Minesweeper [by Vinicius Jardim]\n");
    printf("====================================\n\n");

    printf("Instructions:\n");
    printf("   Move        To reveal, type the number of the square " GREEN "[ ]" DEFAULT_COLOR " you want.\n");
    printf("               That number is number on the row puls the number on the col.\n");
    printf("   Mark        To mark a mine, type an asterisk " GREEN "*" DEFAULT_COLOR ", then do the same\n");
    printf("               procedure as above to find the square number.\n\n");

    printf("Commands:\n");
    printf("   n           Starts a new game. Can be typed at the end or during\n");
    printf("               a match. If typed during a match, that match is canceled.\n");
    printf("   q           Quit the game, returning to the command line.\n");
    printf("   h           Shows this help screen.\n\n");

    printf("Execution (game can start with the following parameters):\n");
    printf("   -row   num  Number of rows.\n");
    printf("   -col   num  Number of columns.\n");
    printf("   -mine  num  Number of mines.\n\n");

    printf("Version: 0.11; July 2011.\n");
    printf("Press " GREEN "r" DEFAULT_COLOR " to return: ");
    char* c = (char*)malloc(sizeof(char));
    scanf("%s", c);
    free(c);
}

// Reveal the entire field
void revealGame(IntMatrix* screen) {
    int i, j;

    for (i = 0; i < screen->rows; i++) {
        for (j = 0; j < screen->cols; j++) {
            screen->mat[i][j] = OPENED;
        }
    }
}

// Interpret commands typed by the player
enum gameConsts interpreter(IntMatrix* field, IntMatrix* screen, char* command, int firstMove, int* bombCount) {
    char subStr[10];
    int openPos;

    if (command[0] == 'n') {
        return NEWGAME;
    }
    else if (command[0] == 'q') {
        return EXIT;
    }
    else if (command[0] == 'h') {
        printHelp();
        return ERROR;
    }
    // Secret cheat code to force a win
    else if (strcmp(command, "> fwin\n") == 0) {
        return WON;
    }
    else if (command[0] == '*') {
        strncpy(subStr, command + 1, 9);
        // Ensure null-termination
        subStr[9] = '\0';

        if (strIsDigit(subStr)) {
            openPos = atoi(subStr);
            if (openPos < field->rows * field->cols && openPos >= 0)
                markCell(field, screen, openPos);

            return ERROR;
        }
    }
    else if (strIsDigit(command)) {
        openPos = atoi(command);

        if (openPos < field->rows * field->cols && openPos >= 0) {
            if (firstMove) {
                initField(field, *bombCount, openPos);
            }

            return openCell(field, screen, openPos);
        }
    }

    return ERROR;
}

// Deallocate field, screen, and command
void deallocate(IntMatrix* field, IntMatrix* screen, char* command) {
    freeIntMatrix(field);
    field = NULL;
    freeIntMatrix(screen);
    screen = NULL;
    free(command);
    command = NULL;
}

// Count flags on the screen
int countFlags(IntMatrix* screen) {
    int total = 0;
    int i, j;

    for (i = 0; i < screen->rows; i++) {
        for (j = 0; j < screen->cols; j++) {
            if (screen->mat[i][j] == FLAG) total++;
        }
    }

    return total;
}

// Main menu loop
enum gameConsts menuGame(int *rows, int *cols, int *bombCount) {
    IntMatrix* field = newIntMatrix(*rows, *cols);
    IntMatrix* screen = newIntMatrix(*rows, *cols);

    enum gameConsts result;
    char* command = (char*)malloc(10 * sizeof(char));
    int firstMove = TRUE;
    time_t startTime;

    while (TRUE) {
        int nFlags = countFlags(screen);
        printGame(field, screen);
        printf("\n %d/%d mines | " GREEN "h" DEFAULT_COLOR " [help] | " GREEN "q" DEFAULT_COLOR " [quit]\n====================================", nFlags, *bombCount);
        printf("\n Move: ");
        fgets(command, 9, stdin);

        result = interpreter(field, screen, command, firstMove, bombCount);

        if (result == KEEPGOING && firstMove) {
            firstMove = FALSE;
            startTime = time(NULL);
        }
        if (result == NEWGAME || result == EXIT) {
            deallocate(field, screen, command);
            return result;
        } 
        else if (result != KEEPGOING && result != ERROR) {
            break;
        }
    }

    revealGame(screen);
    printGame(field, screen);

    if (result == LOST) {
        printf(RED);
        printf("\n YOU LOST!");
    } else if (result == WON) {
        printf(GREEN);
        printf("\n YOU WON!");
    }

    printf(DEFAULT_COLOR);

    time_t endTime = time(NULL);
    time_t totalTime = endTime - startTime;

    printf(" Time: %dm %ds\n====================================\n", (int)totalTime / 60, (int)totalTime % 60);

    printf(GREEN " n" DEFAULT_COLOR " [new game] | " GREEN "q" DEFAULT_COLOR " [quit]: ");
    scanf("%s", command);

    result = interpreter(field, screen, command, FALSE, bombCount);
    deallocate(field, screen, command);

    return result;
}

int main(int argc, char** argv) {
    srand(time(NULL));

    int rows = 10, cols = 10, bombCount = 10;
    int i, parseError = FALSE;

    // Parse command-line arguments
    for (i = 1; i < argc; i++) {
        if (parseError) break;

        if (strcmp(argv[i], "-row") == 0 && i + 1 < argc) {
            if (strIsDigit(argv[i + 1]))
                rows = atoi(argv[i + 1]);
            else
                parseError = TRUE;
        }
        else if (strcmp(argv[i], "-col") == 0 && i + 1 < argc) {
            if (strIsDigit(argv[i + 1]))
                cols = atoi(argv[i + 1]);
            else
                parseError = TRUE;
        }
        else if (strcmp(argv[i], "-mine") == 0 && i + 1 < argc) {
            if (strIsDigit(argv[i + 1]))
                bombCount = atoi(argv[i + 1]);
            else
                parseError = TRUE;
        }
    }

    // Validate bomb count and start the menu
    if (parseError == FALSE) {
        if (bombCount > rows * cols - 10)
            bombCount = rows * cols - 10;

        while (menuGame(&rows, &cols, &bombCount) == NEWGAME) {
            // Keep looping if user chooses a new game
        }
    }
    else {
        printf("Invalid parameters, use them as follows:\n\t-row numberOfRows\n\t");
        printf("-col numberOfColumns\n\t-mine numberOfMines\n");
    }

    return (EXIT_SUCCESS);
}
