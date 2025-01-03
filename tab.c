#include <stdio.h>
#include <curses.h>

#define GAME_METRIC_HEIGHT 3

// Funktion zur Initialisierung des Spielfeldes
void initGameBuffer(int start_y, int start_x, int height, int width) {
    // Zeichne Ecken
    mvaddch(start_y, start_x, ACS_ULCORNER);
    mvaddch(start_y, start_x + width - 1, ACS_URCORNER);
    mvaddch(start_y + height - 1, start_x, ACS_LLCORNER);
    mvaddch(start_y + height - 1, start_x + width - 1, ACS_LRCORNER);

    // Zeichne Seiten
    for (int y = 1; y < height - 1; y++) {
        if (y % 2 == 0) {
            // T-Kreuzungen für linke und rechte Seite
            mvaddch(start_y + y, start_x, ACS_LTEE); // Linke T-Kreuzung
            mvaddch(start_y + y, start_x + width - 1, ACS_RTEE); // Rechte T-Kreuzung
        } else {
            // Vertikale Linien für linke und rechte Seite
            mvaddch(start_y + y, start_x, ACS_VLINE); // Linke Linie
            mvaddch(start_y + y, start_x + width - 1, ACS_VLINE); // Rechte Linie
        }
    }

    // Zeichne obere und untere Linie
    for (int x = 1; x < width - 1; x++) {
        if (x % 4 == 0) {
            // T-Kreuzung für obere Linie
            mvaddch(start_y, start_x + x, ACS_TTEE);
            // T-Kreuzung für untere Linie
            mvaddch(start_y + height - 1, start_x + x, ACS_BTEE);
        } else {
            // Horizontale Linie
            mvaddch(start_y, start_x + x, ACS_HLINE);
            mvaddch(start_y + height - 1, start_x + x, ACS_HLINE);
        }
    }

    // Zeichne Kreuzungspunkte und horizontale Trennlinien
    // Mittlere Linien
    for (int y = 1; y <= height - 2; y++) {
        for (int x = 1; x <= width - 2; x++) {
            if (x % 4 == 0 && y % 2 == 0) {
                // Kreuzungspunkt
                mvaddch(start_y + y, start_x + x, ACS_PLUS);
            } else if (y % 2 == 0) {
                // Horizontale Linien
                mvaddch(start_y + y, start_x + x, ACS_HLINE);
            } else if (x % 4 == 0) {
                // Vertikale Linien
                mvaddch(start_y + y, start_x + x, ACS_VLINE);
            } else {
                // Alternativer Hintergrund (optional)
                mvaddch(start_y + y, start_x + x, ' ');
            }
        }
    }
    refresh();
}

// Funktion zur Initialisierung des unteren Bereichs (Game Metric)
void initGameMetricBuffer(int start_y, int start_x, int height, int width, int gameMetricBuffer[][width]) {
    // -------------------------------------------------------------------
    // Füllen des Basis-Feldes mit Leerzeichen
    // -------------------------------------------------------------------
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == width - 1) {
                gameMetricBuffer[y][x] = '\0';  // Nullterminierung statt Zeilenumbruch
            } else {
                gameMetricBuffer[y][x] = ' ';   // Leerzeichen für das restliche Feld
            }
        }
    }

    // Ecken setzen
    gameMetricBuffer[start_y][start_x] = ACS_ULCORNER;                                // Oben links
    gameMetricBuffer[start_y][start_x + width - 1] = ACS_URCORNER;                   // Oben rechts
    gameMetricBuffer[start_y + height - 1][start_x] = ACS_LLCORNER;                  // Unten links
    gameMetricBuffer[start_y + height - 1][start_x + width - 1] = ACS_LRCORNER;      // Unten rechts

    // Linke und rechte Seiten
    for (int y = 1; y < height - 1; y++) {
        gameMetricBuffer[start_y + y][start_x] = ACS_VLINE;                            // Linke Seite
        gameMetricBuffer[start_y + y][start_x + width - 1] = ACS_VLINE;                // Rechte Seite
    }

    // Obere und untere Linie
    for (int x = 1; x < width - 1; x++) {
        gameMetricBuffer[start_y][start_x + x] = ACS_HLINE;                           // Obere Linie
        gameMetricBuffer[start_y + height - 1][start_x + x] = ACS_HLINE;               // Untere Linie
    }
}


int main() {
    initscr();          // Startet NCurses
    cbreak();           // Direkte Eingabe ohne Enter
    noecho();           // Keine Anzeige der Eingabe
    curs_set(0);        // Cursor ausblenden

    int n;

    // Eingabe der Spielfeldgröße
    printw("Geben Sie die Anzahl der Zellen (min 5): ");
    refresh();
    scanw("%d", &n);
    if (n < 5) {
        printw("Ungültige Eingabe. Die Größe muss >= 5 sein.\n");
        refresh();
        getch();
        endwin();
        return 1;
    }

    // Berechnung der Spielfeldgröße
    int fieldWidth = (4 * n) + 1; // Jede Zelle 4 Spalten breit, inkl. Ränder
    int fieldHeight = (2 * n) + 1; // Jede Zelle 2 Zeilen hoch, inkl. Ränder

    // Zeichne das Spielfeld
    initGameBuffer(1, 1, fieldHeight, fieldWidth);

    // Zeichne den unteren Bereich (Game Metric)
    int metricWidth = fieldWidth; // Gleiche Breite wie Spielfeld
    int gameMetricBuffer[GAME_METRIC_HEIGHT][metricWidth]; // Array für Game Metric
    initGameMetricBuffer(fieldHeight + 2, 1, GAME_METRIC_HEIGHT, metricWidth, gameMetricBuffer);

    // Beispielinhalt im Game Metric Bereich
    mvprintw(fieldHeight + 3, 3, "Schiffe: 10");
    mvprintw(fieldHeight + 4, 3, "Treffer: 5");
    mvprintw(fieldHeight + 5, 3, "Fehlschuesse: 2");

    refresh(); // Änderungen anzeigen
    getch();   // Warten auf Benutzereingabe
    endwin();  // Beendet NCurses
    return 0;
}
