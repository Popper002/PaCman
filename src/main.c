// Include delle librerie necessarie
#include "utils/raylib/src/raylib.h" // Libreria grafica raylib
#include "lib/common.h"              // Header con definizioni comuni del progetto
//Define's
#define LIVES 10


// PROTOTYPE'S
bool IsDirectionValid(Vector2 pos, Vector2 dir);
void ResetGame(Vector2 *ghostStartPositions, Vector2 *pacmanPos, int *score , int state); 
// GLOBAL VAR
int pacmanLives = LIVES;
bool gameOver = false;



void ResetGame(Vector2 *ghostStartPositions, Vector2 *pacmanPos, int *score , int state)
{
    // Reset vite e stato
    pacmanLives = LIVES;
    gameOver = false;

    // Reset punteggio
    *score = 0;

    // Reset mappa
    char originalMap[MAP_ROWS][MAP_COLS] = {
        "###############",
        "#.............#",
        "#.###.###.###.#",
        "#.............#",
        "#.###.#.#.###.#",
        "#.............#",
        "#.###.###.###.#",
        "#.............#",
        "#.###########.#",
        "###############"};

    for (int row = 0; row < MAP_ROWS; row++)
    {
        for (int col = 0; col < MAP_COLS; col++)
        {
            map[row][col] = originalMap[row][col];
        }
    }

    // Reset posizione Pacman
    *pacmanPos = (Vector2){1 * TILE_SIZE + TILE_SIZE / 2.0f, 1 * TILE_SIZE + TILE_SIZE / 2.0f};

    // Reset fantasmi
    for (int i = 0; i < NUM_GHOST; i++)
    {
        ghosts[i].pos = ghostStartPositions[i];
        ghosts[i].dir = (Vector2){GetRandomValue(-1, 1), GetRandomValue(-1, 1)};
        while (ghosts[i].dir.x == 0 && ghosts[i].dir.y == 0)
        {
            ghosts[i].dir = (Vector2){GetRandomValue(-1, 1), GetRandomValue(-1, 1)};
        }
    }
    // with this we remove a lot of duplicated code 
    if(state == QUIT)
    {
        exit(EXIT_SUCCESS); 
    }
}

// Definizione della mappa del gioco
// '#' = muro, '.' = puntino da mangiare, ' ' = spazio vuoto
char map[MAP_ROWS][MAP_COLS] = {
    "###############", // Riga 0: bordo superiore
    "#.............#", // Riga 1: corridoio con puntini
    "#.###.###.###.#", // Riga 2: muri interni
    "#.............#", // Riga 3: corridoio con puntini
    "#.###.#.#.###.#", // Riga 4: muri interni complessi
    "#.............#", // Riga 5: corridoio con puntini
    "#.###.###.###.#", // Riga 6: muri interni
    "#.............#", // Riga 7: corridoio con puntini
    "#.###########.#", // Riga 8: muri interni
    "###############"  // Riga 9: bordo inferiore
};

/* funzione che dice sostanzialmente questo
    Dati due vettori uno posizione attuale e uno la direzione verso cui va il fantasma
*   Se esso è compreso in lunghezza tra 0 e MAP_ROWS ( 0 e n stessa cosa) e
*   lo stesso in altezza (sempre matriciale ), restituisci la posizione (frame valido ) in cui non vi è un muro ovvero un #
*   Altrimento falso --> sta direzione non è corretta (tipo fuori mappa o scontri tra tutti muri )
*/
bool IsDirectionValid(Vector2 pos, Vector2 dir)
{
    int col = (int)(pos.x + dir.x * TILE_SIZE) / TILE_SIZE;
    int row = (int)(pos.y + dir.y * TILE_SIZE) / TILE_SIZE;

    if (row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS)
    {
        return map[row][col] != '#';
    }
    return false;
}

// CheckPacmanCollision: Check if the current position of Pacman is equal to the current position of a ghost

bool CheckPacmanCollision(Vector2 pacmanPos, Vector2 ghostPos)
{
    return (fabs(pacmanPos.x - ghostPos.x) < TILE_SIZE * 0.75f &
            fabs(pacmanPos.y - ghostPos.y) < TILE_SIZE * 0.75f);
}

// Funzione principale del gioco
int main(void)
{
    // Configurazione della finestra di gioco
    const int screenWidth = 600;  // Larghezza della finestra
    const int screenHeight = 400; // Altezza della finestra

    // Inizializza la finestra di raylib
    InitWindow(screenWidth, screenHeight, "Pacman - raylib");
    SetTargetFPS(60); // Imposta il gioco a 60 FPS

    // === INIZIALIZZAZIONE PACMAN ===
    // Posizione iniziale di Pacman (centro della prima cella libera)
    Vector2 pacmanPos = {1 * TILE_SIZE + TILE_SIZE / 2.0f, 1 * TILE_SIZE + TILE_SIZE / 2.0f};
    float pacmanRadius = 20.0f; // Raggio del cerchio di Pacman
    float speed = 3.0f;         // Velocità di movimento (pixel per frame)
    int score = 0;              // Punteggio del giocatore

    // === INIZIALIZZAZIONE FANTASMI ===
    // Posizioni di partenza dei fantasmi (centrate nelle celle)
    Vector2 ghostStartPositions[NUM_GHOST] = {
        {7 * TILE_SIZE + TILE_SIZE / 2.0f, 5 * TILE_SIZE + TILE_SIZE / 2.0f}, // Fantasma 1
        {7 * TILE_SIZE + TILE_SIZE / 2.0f, 4 * TILE_SIZE + TILE_SIZE / 2.0f}, // Fantasma 2
        {6 * TILE_SIZE + TILE_SIZE / 2.0f, 5 * TILE_SIZE + TILE_SIZE / 2.0f}, // Fantasma 3
        {8 * TILE_SIZE + TILE_SIZE / 2.0f, 5 * TILE_SIZE + TILE_SIZE / 2.0f}  // Fantasma 4
    };

    // Colori dei fantasmi (rosso, verde, blu, viola)
    Color ghostColors[NUM_GHOST] = {RED, GREEN, BLUE, PURPLE};

    // Inizializza ogni fantasma con posizione, direzione e colore
    for (int i = 0; i < NUM_GHOST; i++)
    {
        ghosts[i].pos = ghostStartPositions[i];                                  // Imposta posizione iniziale
        ghosts[i].dir = (Vector2){GetRandomValue(-1, 1), GetRandomValue(-1, 1)}; // Direzione iniziale: random
        while (ghosts[i].dir.x == 0 && ghosts[i].dir.y == 0)
        {
            ghosts[i].dir = (Vector2){GetRandomValue(-1, 1), GetRandomValue(-1, 1)};
        }
        ghosts[i].color = ghostColors[i]; // Assegna colore
    }

    // === CICLO PRINCIPALE DEL GIOCO ===
    while (!WindowShouldClose()) // Continua fino a quando la finestra non viene chiusa
    {

        // GameOver implementation
        if (gameOver)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("GAMEOVER", screenWidth / 2 - MeasureText("GAMEOVER", 40) / 2, screenHeight / 2 - 20, 40, RED);
            DrawText(TextFormat("Final Score: %d", score), screenWidth / 2 - MeasureText("Final Score: 9999", 20) / 2, screenHeight / 2 + 30, 20, WHITE);
            DrawText(TextFormat("Lives: %d", pacmanLives), 10, 35, 20, WHITE);
            // Pulsante Restart centrato
            int btnWidth = 140;
            int btnHeight = 40;
            int btnX = screenWidth / 2 - btnWidth / 2;
            int btnY = screenHeight / 2 + 80;
            // create an btn istance
            Rectangle resetBtn = {btnX, btnY, btnWidth, btnHeight};
            // draw on screen the btn
            DrawRectangleRec(resetBtn, DARKGRAY);

            int textWidth = MeasureText("Restart", 20);
            int textX = btnX + (btnWidth - textWidth) / 2;
            int textY = btnY + (btnHeight - 20) / 2;
            // text inside the btn.
            DrawText("Restart", textX, textY, 20, WHITE);
             // add lisner
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), resetBtn))
            {
                ResetGame(ghostStartPositions, &pacmanPos, &score,RESTART);
            }

            // Pulsante Exit sotto il Restart
            int exitBtnY = btnY + btnHeight + 20; // 20px di spazio sotto Restart
            Rectangle exitBtn = {btnX, exitBtnY, btnWidth, btnHeight};
            DrawRectangleRec(exitBtn, DARKGRAY);

            // Testo centrato nel pulsante Exit
            int exitTextWidth = MeasureText("EXIT", 20);
            int exitTextX = btnX + (btnWidth - exitTextWidth) / 2;
            int exitTextY = exitBtnY + (btnHeight - 20) / 2;
            DrawText("EXIT", exitTextX, exitTextY, 20, WHITE);
            
            // add lisner
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(),exitBtn))
            {
                ResetGame(ghostStartPositions, &pacmanPos, &score,QUIT);
            }
           
            // DrawText(TextFormat("@Popper002-RiccardoOro"),-10,35,20,YELLOW);
            EndDrawing();
            continue;
        }
        // === GESTIONE INPUT ===
        // Calcola la prossima posizione di Pacman basata sui tasti premuti
        Vector2 nextPos = pacmanPos; // Inizia dalla posizione corrente
        if (IsKeyDown(KEY_RIGHT))
            nextPos.x += speed; // Muovi verso destra
        if (IsKeyDown(KEY_LEFT))
            nextPos.x -= speed; // Muovi verso sinistra
        if (IsKeyDown(KEY_UP))
            nextPos.y -= speed; // Muovi verso l'alto
        if (IsKeyDown(KEY_DOWN))
            nextPos.y += speed; // Muovi verso il basso

        // === COLLISION DETECTION CON I MURI ===
        // Converte la posizione in pixel alle coordinate della mappa
        int mapCol = (int)(nextPos.x) / TILE_SIZE; // Colonna nella mappa
        int mapRow = (int)(nextPos.y) / TILE_SIZE; // Riga nella mappa

        // Controlla se la posizione è valida e non è un muro
        if (mapRow >= 0 && mapRow < MAP_ROWS && // Dentro i limiti verticali
            mapCol >= 0 && mapCol < MAP_COLS && // Dentro i limiti orizzontali
            map[mapRow][mapCol] != '#')         // Non è un muro
        {
            pacmanPos = nextPos; // Aggiorna la posizione di Pacman

            // === MECCANICA DI RACCOLTA PUNTINI ===
            // Se Pacman è su un puntino, lo mangia
            if (map[mapRow][mapCol] == '.')
            {
                map[mapRow][mapCol] = ' '; // Rimuovi il puntino dalla mappa
                score += 10;               // Incrementa il punteggio
            }
        }

        // === LOGICA DEI FANTASMI ===
        // Muove ogni fantasma e gestisce le collisioni
        for (int i = 0; i < NUM_GHOST; i++)
        {
            Ghost *g = &ghosts[i]; // Puntatore al fantasma

            // Calcola le coordinate della cella corrente del fantasma
            int ghostCol = (int)(g->pos.x) / TILE_SIZE;
            int ghostRow = (int)(g->pos.y) / TILE_SIZE;

            // Calcola le coordinate della cella corrente di Pacman
            int pacCol = (int)(pacmanPos.x) / TILE_SIZE;
            int pacRow = (int)(pacmanPos.y) / TILE_SIZE;

            // Se il fantasma è allineato su una cella (per evitare continui cambi direzione a metà cella)
            if ((int)g->pos.x % TILE_SIZE == TILE_SIZE / 2 && (int)g->pos.y % TILE_SIZE == TILE_SIZE / 2)
            {
                Vector2 directions[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
                Vector2 bestDir = g->dir; // Direzione attuale come default
                float bestDistance = 999999.0f;

                for (int d = 0; d < 4; d++)
                {
                    Vector2 testDir = directions[d];
                    if (testDir.x == -g->dir.x && testDir.y == -g->dir.y)
                        continue; // Evita inversioni

                    if (IsDirectionValid(g->pos, testDir))
                    {
                        Vector2 nextPos = {
                            g->pos.x + testDir.x * TILE_SIZE,
                            g->pos.y + testDir.y * TILE_SIZE};
                        float dx = (pacmanPos.x - nextPos.x);
                        float dy = (pacmanPos.y - nextPos.y);
                        float dist = dx * dx + dy * dy;

                        if (dist < bestDistance)
                        {
                            bestDistance = dist;
                            bestDir = testDir;
                        }
                    }
                }

                g->dir = bestDir;
            }

            // Muove il fantasma nella nuova direzione
            Vector2 nextPos = {
                g->pos.x + g->dir.x * speed,
                g->pos.y + g->dir.y * speed};

            int col = (int)(nextPos.x) / TILE_SIZE;
            int row = (int)(nextPos.y) / TILE_SIZE;

            if (map[row][col] != '#')
            {
                g->pos = nextPos;
            }
            else
            {
                g->dir.x *= -1;
                g->dir.y *= -1;
            }
        }

        for (int i = 0; i < NUM_GHOST; i++)
        {
            if (CheckPacmanCollision(pacmanPos, ghosts[i].pos))
            {
                pacmanLives--;
                if (pacmanLives == 0)
                {
                    gameOver = true;
                }
                else
                {
                    // Reset Pacman e fantasmi

                    pacmanPos = (Vector2){1 * TILE_SIZE + TILE_SIZE / 2.0f, 1 * TILE_SIZE + TILE_SIZE / 2.0f};

                    for (int j = 0; j < NUM_GHOST; j++)
                    {
                        ghosts[j].pos = ghostStartPositions[j];
                        ghosts[j].dir = (Vector2){GetRandomValue(-1, 1), GetRandomValue(-1, 1)};
                        while (ghosts[j].dir.x == 0 && ghosts[j].dir.y == 0)
                        {
                            ghosts[j].dir = (Vector2){GetRandomValue(-1, 1), GetRandomValue(-1, 1)};
                        }
                    }
                }
                break;
            }
        }

        // === RENDERING ===
        BeginDrawing();         // Inizia il frame di rendering
        ClearBackground(BLACK); // Pulisce lo schermo con sfondo nero

        // === DISEGNO DELLA MAPPA ===
        // Itera attraverso ogni cella della mappa
        for (int row = 0; row < MAP_ROWS; row++)
        {
            for (int col = 0; col < MAP_COLS; col++)
            {
                char cell = map[row][col]; // Carattere della cella corrente
                int x = col * TILE_SIZE;   // Posizione X in pixel
                int y = row * TILE_SIZE;   // Posizione Y in pixel

                if (cell == '#')                                               // Se è un muro
                    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, DARKBLUE);       // Disegna rettangolo blu
                else if (cell == '.')                                          // Se è un puntino
                    DrawCircle(x + TILE_SIZE / 2, y + TILE_SIZE / 2, 5, GOLD); // Disegna cerchio dorato
                // Le celle vuote (' ') non vengono disegnate (rimangono nere)
            }
        }

        // === DISEGNO DEI FANTASMI ===
        // Disegna ogni fantasma come un cerchio colorato
        for (int i = 0; i < NUM_GHOST; i++)
        {
            DrawCircleV(ghosts[i].pos, pacmanRadius, ghosts[i].color);
        }

        // === DISEGNO DI PACMAN ===
        // Disegna Pacman come un cerchio giallo
        DrawCircleV(pacmanPos, pacmanRadius, YELLOW);

        // === INTERFACCIA UTENTE ===
        // Mostra il punteggio nell'angolo superiore sinistro
        DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
        // Vite in alto a destra
        const char* livesText = TextFormat("Lives: %d", pacmanLives);
        int livesTextWidth = MeasureText(livesText, 20);
        DrawText(livesText, screenWidth - livesTextWidth - 10, 10, 20, WHITE);

        EndDrawing(); // Termina il frame di rendering
    }

    // === PULIZIA E CHIUSURA ===
    CloseWindow(); // Chiude la finestra e libera le risorse
    return 0;      // Termina il programma con successo
}
