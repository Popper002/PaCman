
#include "utils/raylib/src/raylib.h"
#include "lib/common.h" 

// Definizione della mappa del gioco
char map[MAP_ROWS][MAP_COLS] = { 
    "###############",
    "#.............#",
    "#.###.###.###.#",
    "#.............#",
    "#.###.#.#.###.#",
    "#.............#",
    "#.###.###.###.#",
    "#.............#",
    "#.###########.#",
    "###############"
}; 
int main(int argc, char const *argv[])
{
    // Inizializzazione
    // Dimensione dello schermo in pixel, costanti perchè la finestra sarà sempre grande quella dimensione 
    const int screenWidth = 800;
    const int screenHeight = 450;
    //creo la finestra con le dimensioni e il suo titolo 
    InitWindow(screenWidth, screenHeight, "PaCman - raylib");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    //Struct che rappresenta la posizione bidimensionale inizializzata al centro dello schermo 
    Vector2 pacmanPos = { 1 * TILE_SIZE + TILE_SIZE / 2.0f, 1 * TILE_SIZE + TILE_SIZE / 2.0f }; //dimensione del vettore di pacman 
   
    float pacmanRadius = 20.0f; //come arrotondare 
   
    float speed = 3.0f; //velocità di movimento ogni fram = 3px 

    // Main game loop - ogni iterazione è un frame 
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        // IsKeyDown  = true. significa che il quando il tasto passato in input è premuto fai qualcosa 
        if(IsKeyDown(KEY_RIGHT)) pacmanPos.x +=speed; //destra 
        if(IsKeyDown(KEY_LEFT))  pacmanPos.x -=speed; //sinestra 
        if(IsKeyDown(KEY_UP))    pacmanPos.y -=speed; // su 
        if(IsKeyDown(KEY_DOWN))  pacmanPos.y +=speed; //giu 
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        //Pulizia frame - setup finestra o flush 
        BeginDrawing();

            //Colore per il background
            ClearBackground(BLACK); //Importante: se non lo fai, i disegni si "accumulano".

            for (int row = 0; row< MAP_ROWS; row++)
            {
                for(int col = 0; col < MAP_COLS; col++)
                {
                    char cell = map[row][col]; 
                    int x = col * TILE_SIZE;
                    int y = row * TILE_SIZE;
                    if(cell=='#')
                    {
                        DrawRectangle(x , y , TILE_SIZE, TILE_SIZE, DARKBLUE); 

                    }else if(cell == '.'){ 
                        DrawCircle(x + TILE_SIZE / 2 , y + TILE_SIZE / 2, 5 , GOLD); 
                    }

                }
            }
            
            //stampa una 
            DrawCircleV(pacmanPos, pacmanRadius, YELLOW); // il cerchio è rappresentato dal vettore pacam (la sua posizione sullo schermo) , quanto rotondo, e il colore. 

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
