#ifndef COMMON_H
#define COMMON_H

#include "../utils/raylib/src/raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#define MAP_ROWS 20
#define MAP_COLS 35 
#define TILE_SIZE 40 
#define NUM_GHOST 4

// === STATI DEL GIOCO ===
// Definisce le diverse schermate del gioco
typedef enum {
    GAME_STATE_HOME = 0,      // Schermata iniziale/menu principale
    GAME_STATE_PLAYING,       // Gioco in corso
    GAME_STATE_PAUSED,        // Gioco in pausa
    GAME_STATE_GAME_OVER,     // Schermata di game over
    GAME_STATE_INSTRUCTIONS   // Schermata istruzioni
} GameState;

#define QUIT 1
#define RESTART 0  

// === CONFIGURAZIONE POWER-UP ===
// Definizioni delle costanti per i power-up
#define MAX_POWERUPS 3             // Massimo numero di power-up simultanei sulla mappa
#define POWERUP_SPAWN_CHANCE 100    // 1 su 100 frame (circa ogni 1.7 secondi a 60 FPS)
#define POWERUP_DURATION 300        // Durata effetti: 5 secondi a 60 FPS (300 frame)

// === ENUMERAZIONE DEI TIPI DI POWER-UP ===
// Definisce tutti i tipi di power-up disponibili nel gioco
typedef enum {
    POWERUP_NONE = 0,     // Nessun power-up (valore di default)
    POWERUP_SPEED,        // Aumenta velocità di Pacman del 50%
    POWERUP_INVINCIBLE,   // Rende Pacman invincibile ai fantasmi
    POWERUP_SCORE_BOOST,  // Raddoppia i punti ottenuti dai puntini
    POWERUP_SLOW_GHOSTS,  // Rallenta i fantasmi del 50%
    POWERUP_EXTRA_LIFE    // Aggiunge una vita extra
} PowerUpType;

// === STRUTTURA POWER-UP SULLA MAPPA ===
// Rappresenta un power-up fisicamente presente sulla mappa di gioco
typedef struct {
    Vector2 pos;        // Posizione del power-up sulla mappa (in pixel)
    PowerUpType type;   // Tipo di power-up (velocità, invincibilità, ecc.)
    bool isActive;      // Se true, il power-up è visibile e raccoglibile
    int spawnTime;      // Momento in cui il power-up è apparso (in frame)
    Color color;        // Colore di rendering del power-up
} PowerUp;

// === STRUTTURA POWER-UP ATTIVO ===
// Rappresenta un effetto power-up attualmente in corso su Pacman
typedef struct {
    PowerUpType type;   // Tipo di power-up attivo
    int timeLeft;       // Tempo rimanente dell'effetto (in frame)
} ActivePowerUp;

// Dichiarazione esterna della mappa (definita in main.c)
extern char map[MAP_ROWS][MAP_COLS]; 

// === STRUTTURA FANTASMA ===
// Rappresenta un fantasma nemico nel gioco
typedef struct 
{
    Vector2 pos;        // Posizione attuale del fantasma (in pixel)
    Vector2 dir;        // Direzione di movimento (-1, 0, 1 per x e y)
    Color color;        // Colore di rendering del fantasma
} Ghost;

// === VARIABILI GLOBALI ===
extern Ghost ghosts[NUM_GHOST];  // Array dei fantasmi del gioco

// === FUNZIONI UTILITY ===
// Calcola la distanza euclidea tra due punti (alternativa a Vector2Distance di raylib)
float CalculateDistance(Vector2 v1, Vector2 v2);

// === FUNZIONI DI GESTIONE POWER-UP ===
// Inizializza il sistema dei power-up (chiamata all'inizio del gioco)
void InitializePowerUps(void);

// Genera casualmente un nuovo power-up sulla mappa
void SpawnPowerUp(void);

// Controlla se Pacman ha raccolto un power-up e applica l'effetto
void CheckPowerUpCollection(Vector2 pacmanPos, int *score, int *lives);

// Applica immediatamente l'effetto di un power-up
void ApplyPowerUp(PowerUpType type, int *score, int *lives);

// Aggiunge un power-up alla lista degli effetti attivi
void AddActivePowerUp(PowerUpType type, int duration);

// Aggiorna tutti i power-up attivi (decrementa i timer)
void UpdateActivePowerUps(void);

// Verifica se un determinato tipo di power-up è attualmente attivo
bool IsPowerUpActive(PowerUpType type);

// Restituisce il moltiplicatore di velocità per Pacman
float GetSpeedMultiplier(void);

// Restituisce il moltiplicatore di punteggio per i puntini
int GetScoreMultiplier(void);

// Verifica se Pacman è attualmente invincibile
bool IsInvincible(void);

// Disegna tutti i power-up presenti sulla mappa
void DrawPowerUps(void);

// Disegna gli indicatori degli effetti attivi nell'interfaccia
void DrawActivePowerUpIndicators(void);

// === FUNZIONI DI GESTIONE SCHERMATE ===
// Disegna la schermata iniziale/menu
void DrawHomeScreen(int screenWidth, int screenHeight);

// Disegna la schermata delle istruzioni
void DrawInstructionsScreen(int screenWidth, int screenHeight);

// Gestisce l'input nella schermata home
GameState HandleHomeInput(void);

// Gestisce l'input nella schermata istruzioni
GameState HandleInstructionsInput(void);

// === FUNZIONI PRINCIPALI DEL GIOCO ===
// Inizializza il sistema di gioco di Pacman
void InitPacman(void);

// Aggiorna la logica di gioco (spawn power-up, aggiorna effetti)
void UpdatePacman(void);

// Disegna gli elementi di gioco (power-up, effetti visivi)
void DrawPacman(void);

#endif // COMMON_H
