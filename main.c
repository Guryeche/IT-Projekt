#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>

int max_size = 8;
int SHIP_COUNT = 8;

// Spielfeldstruktur (mit dynamischer Allokation)
char **player_board;
char **computer_board;
int **player_hit_board;
int **computer_hit_board;

// Schiffarten
int ship_sizes[] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};

int cursorX = 0, cursorY = 0;

int ASCII_A = (int) 'A';

// Farben initialisieren
void init_colors() {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);   // Wasser
    init_pair(2, COLOR_WHITE, COLOR_BLACK);  // Treffer
    init_pair(3, COLOR_WHITE, COLOR_RED);    // Fehlschuss
    init_pair(4, COLOR_BLACK, COLOR_WHITE);  // Schiff
    init_pair(5, COLOR_YELLOW, COLOR_BLACK); // Text
    init_pair(6, COLOR_BLACK, COLOR_YELLOW); // Cursor
}

// Spielfeld initialisieren
void init_board(char **board, int **hit_board) {
    for (int i = 1; i < max_size; i++) {
        for (int j = 1; j < max_size; j++) {
            board[i][j] = '~';
            hit_board[i][j] = 0;
        }
    }
}

// Spielfeld zeichnen
void draw_board(char **board, int **hit_board) {
    clear();
    for (int i = 0; i < max_size; i++) {
        
            if (i == 0) {
               printw("%s ", "  ");
            } else {
               printw("%2d ", i);
            }

        for (int j = 0; j < max_size; j++) {
        
            if (i == 0 && j< (max_size - 1) ) {
                printw("%c ", (char)(ASCII_A + j));
            } else if (cursorX == i && cursorY == j) {
                attron(COLOR_PAIR(6));
                printw("X ");
                attroff(COLOR_PAIR(6));
            } else if (hit_board[i][j] == 1) {
                attron(COLOR_PAIR(2));
                printw("X ");
                attroff(COLOR_PAIR(2));
            } else if (hit_board[i][j] == -1) {
                attron(COLOR_PAIR(3));
                printw("O ");
                attroff(COLOR_PAIR(3));
            } else {
                attron(COLOR_PAIR(1));
                printw("%c ", board[i][j]);
                attroff(COLOR_PAIR(1));
            }
        }
        printw("\n");
    }
    refresh();
}

// Spiel speichern
void save_game() {
    FILE *file = fopen("battleship_save.dat", "wb");
    if (!file) {
        printw("Fehler beim Speichern des Spiels!\n");
        return;
    }
    fwrite(player_board[0], sizeof(char), max_size * max_size, file);
    fwrite(computer_board[0], sizeof(char), max_size * max_size, file);
    fwrite(player_hit_board[0], sizeof(int), max_size * max_size, file);
    fwrite(computer_hit_board[0], sizeof(int), max_size * max_size, file);
    fclose(file);
    printw("Spiel erfolgreich gespeichert!\n");
    refresh();
    getch();
}

// Spiel laden
void load_game() {
    FILE *file = fopen("battleship_save.dat", "rb");
    if (!file) {
        printw("Keine gespeicherte Datei gefunden!\n");
        return;
    }
    fread(player_board[0], sizeof(char), max_size * max_size, file);
    fread(computer_board[0], sizeof(char), max_size * max_size, file);
    fread(player_hit_board[0], sizeof(int), max_size * max_size, file);
    fread(computer_hit_board[0], sizeof(int), max_size * max_size, file);
    fclose(file);
    printw("Spiel erfolgreich geladen!\n");
    refresh();
    getch();
}

// Computerzug
void computer_turn() {
    int x, y;
    do {
        x = rand() % max_size;
        y = rand() % max_size;
    } while (player_hit_board[x][y] != 0);

    if (player_board[x][y] == 'S') {
        player_hit_board[x][y] = 1;
        printw("Computer trifft bei %c%d!\n", 'A' + y, x + 1);
    } else {
        player_hit_board[x][y] = -1;
        printw("Computer verfehlt bei %c%d!\n", 'A' + y, x + 1);
    }
    refresh();
    getch();
}

int main() {
    srand(time(NULL));
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    init_colors();

    int option = -1;
    do {
        printw("####################################### \n");
        printw("# Waehlen Sie bitte die Feld groesse: #\n");
        printw("# 1. Klein: 8 x 8                     #\n");
        printw("# 2. Mittel: 10 x 10                  #\n");
        printw("# 3. Gross: 12 x 12                   #\n");
        printw("#######################################\n");
        option = (int)getch() - 48;
        if (!(option > 0 && option < 4)) {
            printw("Die Eingabe ist falsch. Bitte nur 1, 2 oder 3 auswaehlen!\n");
            continue;
        }

    } while (!(option > 0 && option < 4));

    if (option == 1) {
        max_size = 8;
    } else if (option == 2) {
        max_size = 10;
    } else if (option == 3) {
        max_size = 12;
    }

    max_size++;

    // Dynamische Allokation der Spielfelder
    player_board = (char**)malloc(max_size * sizeof(char*));
    computer_board = (char**)malloc(max_size * sizeof(char*));
    player_hit_board = (int**)malloc(max_size * sizeof(int*));
    computer_hit_board = (int**)malloc(max_size * sizeof(int*));

    // Speicher für jedes Feld allokieren
    for (int i = 0; i < max_size; i++) {
        player_board[i] = (char*)malloc(max_size * sizeof(char));
        computer_board[i] = (char*)malloc(max_size * sizeof(char));
        player_hit_board[i] = (int*)malloc(max_size * sizeof(int));
        computer_hit_board[i] = (int*)malloc(max_size * sizeof(int));
    }

    init_board(player_board, player_hit_board);
    init_board(computer_board, computer_hit_board);

    int ch;
    while (1) {
        draw_board(computer_board, computer_hit_board);
        ch = getch();
        switch (ch) {
            case KEY_UP:
                if (cursorX > 0) cursorX--;
                break;
            case KEY_DOWN:
                if (cursorX < max_size - 1) cursorX++;
                break;
            case KEY_LEFT:
                if (cursorY > 0) cursorY--;
                break;
            case KEY_RIGHT:
                if (cursorY < max_size - 1) cursorY++;
                break;
            case 's':
                save_game();
                break;
            case 'l':
                load_game();
                break;
            default:
                if (computer_board[cursorX][cursorY] == 'S') {
                    computer_hit_board[cursorX][cursorY] = 1;
                    printw("Treffer!\n");
                } else {
                    computer_hit_board[cursorX][cursorY] = -1;
                    printw("Fehlschuss!\n");
                }
                computer_turn();
                break;
        }
    }

    // Freigeben des dynamisch allozierten Speichers
    for (int i = 0; i < max_size; i++) {
        free(player_board[i]);
        free(computer_board[i]);
        free(player_hit_board[i]);
        free(computer_hit_board[i]);
    }

    free(player_board);
    free(computer_board);
    free(player_hit_board);
    free(computer_hit_board);

    endwin();
    return 0;
}
