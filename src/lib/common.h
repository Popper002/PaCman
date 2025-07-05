#include "../utils/raylib/src/raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAP_ROWS 20
#define MAP_COLS 35 
#define TILE_SIZE 40 
#define NUM_GHOST 3
// Dichiarazione esterna della mappa (definita in main.c)
extern char map[MAP_ROWS][MAP_COLS]; 

typedef struct 
{
    Vector2 pos; 
    Vector2 dir; 
    Color color; 
}Ghost;

Ghost ghosts[NUM_GHOST]; 
