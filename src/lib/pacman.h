#ifndef _PACMAN_H
#define _PACMAN_H
#include "../utils/raylib/src/raylib.h"
#include "common.h"

// Dichiarazioni delle funzioni per i power-up
void UpdatePowerUpEffects(void);
void SpawnPowerUp(void);
void InitializePowerUps(void);
void CheckPowerUpCollection(Vector2 pacmanPos, int *score, int *lives);
void ApplyPowerUp(PowerUpType type, int *score, int *lives);
void AddActivePowerUp(PowerUpType type, int duration);
void UpdateActivePowerUps(void);
bool IsPowerUpActive(PowerUpType type);
float GetSpeedMultiplier(void);
int GetScoreMultiplier(void);
bool IsInvincible(void);
void DrawPowerUps(void);
void DrawActivePowerUpIndicators(void);
void InitPacman(void);
void UpdatePacman(void);
void DrawPacman(void);

// Funzioni aggiuntive richieste da main.c
float GetModifiedSpeed(float baseSpeed);
float GetGhostSpeed(float baseSpeed);
bool IsPacmanInvincible(void);
void DrawPowerUpIndicators(int screenWidth);
void DrawPacman(void);
#endif