// === INCLUDE E DICHIARAZIONI ===
#include "utils/raylib/src/raylib.h"
#include "lib/common.h"

/*
 * === SISTEMA POWER-UP DI PACMAN ===
 * 
 * Questo file implementa un sistema completo di power-up per il gioco Pacman.
 * Il sistema funziona su due livelli:
 * 
 * 1. POWER-UP SULLA MAPPA:
 *    - Appaiono casualmente su celle vuote della mappa
 *    - Sono visibili come cerchi colorati
 *    - Possono essere raccolti da Pacman camminandoci sopra
 *    - Massimo 5 power-up simultanei sulla mappa
 * 
 * 2. EFFETTI ATTIVI:
 *    - Una volta raccolti, i power-up applicano effetti temporanei
 *    - Ogni effetto ha una durata limitata (5 secondi)
 *    - Gli effetti si combinano se raccolti più power-up dello stesso tipo
 *    - Indicatori visivi mostrano quali effetti sono attivi
 * 
 * TIPI DI POWER-UP DISPONIBILI:
 * - SPEED: Aumenta la velocità di Pacman del 50%
 * - INVINCIBLE: Rende Pacman invulnerabile ai fantasmi
 * - SCORE_BOOST: Raddoppia i punti ottenuti dai puntini
 * - SLOW_GHOSTS: Rallenta i fantasmi del 50%
 * - EXTRA_LIFE: Aggiunge una vita extra (effetto immediato)
 * 
 * MECCANICA DI SPAWN:
 * - Ogni frame c'è 1% di possibilità che appaia un power-up
 * - I power-up appaiono solo su celle vuote (non su muri o puntini)
 * - Il tipo di power-up è scelto casualmente
 * - Il colore del power-up indica il tipo di effetto
 */

// === VARIABILI GLOBALI PER I POWER-UP ===
PowerUp powerups[MAX_POWERUPS];              // Array dei power-up presenti sulla mappa
ActivePowerUp activePowerUps[MAX_POWERUPS];  // Array dei power-up attualmente attivi
int numActivePowerUps = 0;                   // Numero di power-up attivi

// === FUNZIONI DI INIZIALIZZAZIONE ===

// Inizializza tutti i power-up come inattivi all'avvio del gioco
void InitializePowerUps(void)
{
    // Resetta tutti i power-up sulla mappa
    for (int i = 0; i < MAX_POWERUPS; i++)
    {
        powerups[i].isActive = false;       // Disattiva il power-up
        powerups[i].type = POWERUP_NONE;    // Nessun tipo assegnato
        powerups[i].pos = (Vector2){0, 0};  // Posizione di default
        powerups[i].color = WHITE;          // Colore di default
    }
    numActivePowerUps = 0;                  // Nessun power-up attivo
}

// === FUNZIONI DI SPAWN ===

// Spawna un power-up in una posizione casuale sulla mappa
void SpawnPowerUp(void)
{
    // Cerca uno slot libero nell'array dei power-up
    for (int i = 0; i < MAX_POWERUPS; i++)
    {
        if (!powerups[i].isActive)  // Se lo slot è libero
        {
            // === RICERCA POSIZIONE VALIDA ===
            // Trova una posizione libera casuale (non su muri o puntini)
            int attempts = 0;
            int row, col;
            do
            {
                row = GetRandomValue(1, MAP_ROWS - 2);    // Evita i bordi
                col = GetRandomValue(1, MAP_COLS - 2);    // Evita i bordi
                attempts++;
            } while (map[row][col] != ' ' && attempts < 100);  // Solo su spazi vuoti
            
            if (attempts < 100)  // Se ha trovato una posizione valida
            {
                // === CONFIGURAZIONE POWER-UP ===
                powerups[i].isActive = true;  // Attiva il power-up
                // Centra il power-up nella cella
                powerups[i].pos = (Vector2){
                    col * TILE_SIZE + TILE_SIZE / 2.0f, 
                    row * TILE_SIZE + TILE_SIZE / 2.0f
                };
                
                // Sceglie un tipo casuale di power-up (1-4, escludendo POWERUP_NONE)
                PowerUpType type = GetRandomValue(1, 4);
                powerups[i].type = type;
                
                // Colore basato sul tipo
                switch (type)
                {
                    case POWERUP_SPEED:
                        powerups[i].color = BLUE;
                        break;
                    case POWERUP_INVINCIBLE:
                        powerups[i].color = GOLD;
                        break;
                    case POWERUP_SCORE_BOOST:
                        powerups[i].color = GREEN;
                        break;
                    case POWERUP_EXTRA_LIFE:
                        powerups[i].color = PINK;
                        break;
                    default:
                        powerups[i].color = WHITE;
                        break;
                }
            }
            break;
        }
    }
}

// Controlla se Pacman ha raccolto un power-up
void CheckPowerUpCollection(Vector2 pacmanPos, int *score, int *lives)
{
    for (int i = 0; i < MAX_POWERUPS; i++)
    {
        if (powerups[i].isActive)
        {
            float distance = CalculateDistance(pacmanPos, powerups[i].pos);
            if (distance < 25.0f) // Raggio di raccolta
            {
                // Applica l'effetto del power-up
                ApplyPowerUp(powerups[i].type, score, lives);
                
                // Disattiva il power-up
                powerups[i].isActive = false;
                break;
            }
        }
    }
}

// Applica l'effetto di un power-up
void ApplyPowerUp(PowerUpType type, int *score, int *lives)
{
    switch (type)
    {
        case POWERUP_SPEED:
            // Aggiunge velocità per 5 secondi
            AddActivePowerUp(POWERUP_SPEED, POWERUP_DURATION);
            break;
            
        case POWERUP_INVINCIBLE:
            // Rende invincibile per 5 secondi
            AddActivePowerUp(POWERUP_INVINCIBLE, POWERUP_DURATION);
            break;
            
        case POWERUP_SCORE_BOOST:
            // Raddoppia i punti per 5 secondi
            AddActivePowerUp(POWERUP_SCORE_BOOST, POWERUP_DURATION);
            break;
            
        case POWERUP_EXTRA_LIFE:
            // Vita extra immediata
            (*lives)++;
            *score += 100; // Bonus punti
            break;
            
        default:
            break;
    }
}

// Aggiunge un power-up attivo
void AddActivePowerUp(PowerUpType type, int duration)
{
    // Controlla se il power-up è già attivo
    for (int i = 0; i < numActivePowerUps; i++)
    {
        if (activePowerUps[i].type == type)
        {
            // Rinnova la durata
            activePowerUps[i].timeLeft = duration;
            return;
        }
    }
    
    // Aggiunge nuovo power-up attivo
    if (numActivePowerUps < MAX_POWERUPS)
    {
        activePowerUps[numActivePowerUps].type = type;
        activePowerUps[numActivePowerUps].timeLeft = duration;
        numActivePowerUps++;
    }
}

// Aggiorna i power-up attivi
void UpdateActivePowerUps(void)
{
    for (int i = 0; i < numActivePowerUps; i++)
    {
        activePowerUps[i].timeLeft--;
        
        if (activePowerUps[i].timeLeft <= 0)
        {
            // Rimuovi il power-up scaduto
            for (int j = i; j < numActivePowerUps - 1; j++)
            {
                activePowerUps[j] = activePowerUps[j + 1];
            }
            numActivePowerUps--;
            i--; // Ricontrolla la stessa posizione
        }
    }
}

// Controlla se un power-up è attivo
bool IsPowerUpActive(PowerUpType type)
{
    for (int i = 0; i < numActivePowerUps; i++)
    {
        if (activePowerUps[i].type == type)
        {
            return true;
        }
    }
    return false;
}

// Ottiene il moltiplicatore di velocità
float GetSpeedMultiplier(void)
{
    return IsPowerUpActive(POWERUP_SPEED) ? 1.5f : 1.0f;
}

// Ottiene il moltiplicatore di punteggio
int GetScoreMultiplier(void)
{
    return IsPowerUpActive(POWERUP_SCORE_BOOST) ? 2 : 1;
}

// Controlla se Pacman è invincibile
bool IsInvincible(void)
{
    return IsPowerUpActive(POWERUP_INVINCIBLE);
}

// Disegna i power-up sulla mappa
void DrawPowerUps(void)
{
    for (int i = 0; i < MAX_POWERUPS; i++)
    {
        if (powerups[i].isActive)
        {
            // Effetto pulsante
            float pulse = (sin(GetTime() * 8.0f) + 1.0f) * 0.5f;
            float size = 12.0f + pulse * 5.0f;
            
            DrawCircleV(powerups[i].pos, size, powerups[i].color);
            DrawCircleV(powerups[i].pos, size * 0.7f, WHITE);
            
            // Simbolo del power-up
            const char* symbol = "";
            switch (powerups[i].type)
            {
                case POWERUP_SPEED: symbol = "S"; break;
                case POWERUP_INVINCIBLE: symbol = "I"; break;
                case POWERUP_SCORE_BOOST: symbol = "X"; break;
                case POWERUP_EXTRA_LIFE: symbol = "+"; break;
                default: symbol = "?"; break;
            }
            
            DrawText(symbol, powerups[i].pos.x - 5, powerups[i].pos.y - 8, 16, BLACK);
        }
    }
}

// Disegna gli indicatori dei power-up attivi
void DrawActivePowerUpIndicators(void)
{
    int yOffset = 40;
    for (int i = 0; i < numActivePowerUps; i++)
    {
        const char* name = "";
        Color color = WHITE;
        
        switch (activePowerUps[i].type)
        {
            case POWERUP_SPEED:
                name = "SPEED";
                color = BLUE;
                break;
            case POWERUP_INVINCIBLE:
                name = "INVINCIBLE";
                color = GOLD;
                break;
            case POWERUP_SCORE_BOOST:
                name = "SCORE x2";
                color = GREEN;
                break;
            default:
                name = "UNKNOWN";
                break;
        }
        
        float timePercent = (float)activePowerUps[i].timeLeft / POWERUP_DURATION;
        int barWidth = (int)(100 * timePercent);
        
        DrawText(name, 10, yOffset + i * 25, 16, color);
        DrawRectangle(10, yOffset + i * 25 + 18, 100, 4, DARKGRAY);
        DrawRectangle(10, yOffset + i * 25 + 18, barWidth, 4, color);
    }
}

// Funzione per calcolare la distanza tra due punti
float CalculateDistance(Vector2 v1, Vector2 v2)
{
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    return sqrtf(dx * dx + dy * dy);
}

// Funzioni aggiuntive richieste da main.c
float GetModifiedSpeed(float baseSpeed)
{
    return baseSpeed * GetSpeedMultiplier();
}

float GetGhostSpeed(float baseSpeed)
{
    // I fantasmi vanno più lenti se c'è il power-up SLOW_GHOSTS
    if (IsPowerUpActive(POWERUP_SLOW_GHOSTS))
        return baseSpeed * 0.5f;
    return baseSpeed;
}

bool IsPacmanInvincible(void)
{
    return IsInvincible();
}

void DrawPowerUpIndicators(int screenWidth)
{
    DrawActivePowerUpIndicators();
}

// === FUNZIONI DI GESTIONE SCHERMATE ===

// Disegna la schermata iniziale/menu principale
void DrawHomeScreen(int screenWidth, int screenHeight)
{
    // Sfondo con gradiente scuro
    ClearBackground(BLACK);
    
    // Titolo principale
    const char* title = "PaCman";
    int titleSize = 60;
    int titleWidth = MeasureText(title, titleSize);
    int titleX = screenWidth / 2 - titleWidth / 2;
    int titleY = screenHeight / 4;
    
    // Effetto ombra per il titolo
    DrawText(title, titleX + 3, titleY + 3, titleSize, DARKGRAY);
    DrawText(title, titleX, titleY, titleSize, YELLOW);
    
    // Sottotitolo
    const char* subtitle = "by Riccardo Oro - @Popper002";
    int subtitleSize = 20;
    int subtitleWidth = MeasureText(subtitle, subtitleSize);
    int subtitleX = screenWidth / 2 - subtitleWidth / 2;
    int subtitleY = titleY + titleSize + 10;
    DrawText(subtitle, subtitleX, subtitleY, subtitleSize, GOLD);
    
    // Pulsanti del menu
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonX = screenWidth / 2 - buttonWidth / 2;
    int buttonSpacing = 60;
    int startY = screenHeight / 2 + 20;
    
    // Pulsante "Inizia Gioco"
    Rectangle playButton = {buttonX, startY, buttonWidth, buttonHeight};
    Color playColor = CheckCollisionPointRec(GetMousePosition(), playButton) ? DARKGREEN : GREEN;
    DrawRectangleRec(playButton, playColor);
    DrawRectangleLinesEx(playButton, 2, WHITE);
    
    const char* playText = "INIZIA GIOCO";
    int playTextWidth = MeasureText(playText, 20);
    int playTextX = buttonX + (buttonWidth - playTextWidth) / 2;
    int playTextY = startY + (buttonHeight - 20) / 2;
    DrawText(playText, playTextX, playTextY, 20, WHITE);
    
    // Pulsante "Istruzioni"
    Rectangle instructionsButton = {buttonX, startY + buttonSpacing, buttonWidth, buttonHeight};
    Color instructionsColor = CheckCollisionPointRec(GetMousePosition(), instructionsButton) ? DARKBLUE : BLUE;
    DrawRectangleRec(instructionsButton, instructionsColor);
    DrawRectangleLinesEx(instructionsButton, 2, WHITE);
    
    const char* instructionsText = "ISTRUZIONI";
    int instructionsTextWidth = MeasureText(instructionsText, 20);
    int instructionsTextX = buttonX + (buttonWidth - instructionsTextWidth) / 2;
    int instructionsTextY = startY + buttonSpacing + (buttonHeight - 20) / 2;
    DrawText(instructionsText, instructionsTextX, instructionsTextY, 20, WHITE);
    
    // Pulsante "Esci"
    Rectangle exitButton = {buttonX, startY + buttonSpacing * 2, buttonWidth, buttonHeight};
    Color exitColor = CheckCollisionPointRec(GetMousePosition(), exitButton) ? MAROON : RED;
    DrawRectangleRec(exitButton, exitColor);
    DrawRectangleLinesEx(exitButton, 2, WHITE);
    
    const char* exitText = "ESCI";
    int exitTextWidth = MeasureText(exitText, 20);
    int exitTextX = buttonX + (buttonWidth - exitTextWidth) / 2;
    int exitTextY = startY + buttonSpacing * 2 + (buttonHeight - 20) / 2;
    DrawText(exitText, exitTextX, exitTextY, 20, WHITE);
    
    // Decorazioni: piccoli fantasmi animati
    float time = GetTime();
    int ghostSize = 30;
    
    // Fantasma rosso che si muove
    int redGhostX = 50 + (int)(sinf(time * 2) * 30); // the sinf() funztion computes the sine of x (measured in radians)
    int redGhostY = screenHeight - 80;
    DrawCircle(redGhostX, redGhostY, ghostSize, RED);
    
    // Fantasma blu che si muove
    int blueGhostX = screenWidth - 50 - (int)(sinf(time * 2.5) * 30);
    int blueGhostY = screenHeight - 80;
    DrawCircle(blueGhostX, blueGhostY, ghostSize, BLUE);
    
    // Pacman che "insegue" i fantasmi
    int pacmanX = 150 + (int)(sinf(time * 1.5) * 20);
    int pacmanY = screenHeight - 80;
    DrawCircle(pacmanX, pacmanY, ghostSize, YELLOW);
    
    // Istruzioni in basso
    const char* hint = "Usa il mouse per navigare";
    int hintWidth = MeasureText(hint, 16);
    int hintX = screenWidth / 2 - hintWidth / 2;
    int hintY = screenHeight - 30;
    DrawText(hint, hintX, hintY, 16, LIGHTGRAY);
}

// Disegna la schermata delle istruzioni
void DrawInstructionsScreen(int screenWidth, int screenHeight)
{
    ClearBackground(BLACK);
    
    // Titolo
    const char* title = "ISTRUZIONI";
    int titleSize = 40;
    int titleWidth = MeasureText(title, titleSize);
    int titleX = screenWidth / 2 - titleWidth / 2;
    int titleY = 30;
    DrawText(title, titleX, titleY, titleSize, YELLOW);
    
    // Contenuto delle istruzioni
    int textSize = 18;
    int lineHeight = 25;
    int startY = 100;
    int textX = 50;
    
    DrawText("CONTROLLI:", textX, startY, textSize, WHITE);
    DrawText("• Usa le frecce per muovere Pacman", textX + 20, startY + lineHeight, textSize, LIGHTGRAY);
    DrawText("• Raccogli tutti i puntini gialli", textX + 20, startY + lineHeight * 2, textSize, LIGHTGRAY);
    DrawText("• Evita i fantasmi colorati", textX + 20, startY + lineHeight * 3, textSize, LIGHTGRAY);
    
    DrawText("POWER-UP:", textX, startY + lineHeight * 5, textSize, WHITE);
    DrawText("• Velocità (Giallo): Aumenta la velocità", textX + 20, startY + lineHeight * 6, textSize, YELLOW);
    DrawText("• Invincibilità (Verde): Protegge dai fantasmi", textX + 20, startY + lineHeight * 7, textSize, GREEN);
    DrawText("• Punteggio (Blu): Raddoppia i punti", textX + 20, startY + lineHeight * 8, textSize, BLUE);
    DrawText("• Rallenta fantasmi (Viola): Rallenta i nemici", textX + 20, startY + lineHeight * 9, textSize, PURPLE);
    DrawText("• Vita extra (Rosso): Aggiunge una vita", textX + 20, startY + lineHeight * 10, textSize, RED);
    
    DrawText("OBIETTIVO:", textX, startY + lineHeight * 12, textSize, WHITE);
    DrawText("• Ottieni il punteggio più alto possibile!", textX + 20, startY + lineHeight * 13, textSize, LIGHTGRAY);
    
    // Pulsante "Indietro"
    int buttonWidth = 150;
    int buttonHeight = 40;
    int buttonX = screenWidth / 2 - buttonWidth / 2;
    int buttonY = screenHeight - 80;
    
    Rectangle backButton = {buttonX +220, buttonY, buttonWidth, buttonHeight};
    Color backColor = CheckCollisionPointRec(GetMousePosition(), backButton) ? DARKGRAY : GRAY;
    DrawRectangleRec(backButton, backColor);
    DrawRectangleLinesEx(backButton, 2, WHITE);
    
    const char* backText = "INDIETRO";
    int backTextWidth = MeasureText(backText, 18);
    int backTextX = buttonX + (buttonWidth - backTextWidth) / 2;
    int backTextY = buttonY + (buttonHeight - 18) / 2;
    DrawText(backText, backTextX+220, backTextY, 18, WHITE);
}

// Gestisce l'input nella schermata home
GameState HandleHomeInput(void)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mousePos = GetMousePosition();
        
        // Dimensioni e posizioni dei pulsanti (devono coincidere con DrawHomeScreen)
        int buttonWidth = 200;
        int buttonHeight = 50;
        int buttonX = GetScreenWidth() / 2 - buttonWidth / 2;
        int buttonSpacing = 60;
        int startY = GetScreenHeight() / 2 + 20;
        
        // Check pulsante "Inizia Gioco"
        Rectangle playButton = {buttonX, startY, buttonWidth, buttonHeight};
        if (CheckCollisionPointRec(mousePos, playButton))
        {
            return GAME_STATE_PLAYING;
        }
        
        // Check pulsante "Istruzioni"
        Rectangle instructionsButton = {buttonX, startY + buttonSpacing, buttonWidth, buttonHeight};
        if (CheckCollisionPointRec(mousePos, instructionsButton))
        {
            return GAME_STATE_INSTRUCTIONS;
        }
        
        // Check pulsante "Esci"
        Rectangle exitButton = {buttonX, startY + buttonSpacing * 2, buttonWidth, buttonHeight};
        if (CheckCollisionPointRec(mousePos, exitButton))
        {
            CloseWindow();
            exit(0);
        }
    }
    
    return GAME_STATE_HOME;
}

// Gestisce l'input nella schermata istruzioni
GameState HandleInstructionsInput(void)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mousePos = GetMousePosition();
        
        // Dimensioni e posizioni del pulsante "Indietro"
        // IMPORTANTE: Deve coincidere con la posizione in DrawInstructionsScreen()
        int buttonWidth = 150;
        int buttonHeight = 40;
        int buttonX = GetScreenWidth() / 2 - buttonWidth / 2;
        int buttonY = GetScreenHeight() - 80;
        
        // Correggi la posizione del pulsante (aggiungi +220 come nel rendering)
        Rectangle backButton = {buttonX + 220, buttonY, buttonWidth, buttonHeight};
        if (CheckCollisionPointRec(mousePos, backButton))
        {
            return GAME_STATE_HOME;
        }
    }
    
    // Anche ESC per tornare indietro
    if (IsKeyPressed(KEY_ESCAPE))
    {
        return GAME_STATE_HOME;
    }
    
    return GAME_STATE_INSTRUCTIONS;
}

// === FINE FUNZIONI SCHERMATE ===

// === FUNZIONI PRINCIPALI ===

void InitPacman(void)
{
    // Inizializzazione del gioco PaCman
    InitializePowerUps();
}

void UpdatePacman(void)
{
    // Aggiornamento della logica del gioco
    UpdateActivePowerUps();
    
    // Spawna power-up casualmente
    if (GetRandomValue(1, POWERUP_SPAWN_CHANCE) == 1)
    {
        SpawnPowerUp();
    }
}

void DrawPacman(void)
{
    // Rendering del gioco
    DrawPowerUps();
    DrawActivePowerUpIndicators();
}

