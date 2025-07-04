
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
    
    Vector2 nextPos = pacmanPos; 

    float pacmanRadius = 20.0f; //come arrotondare 
   
    float speed = 3.0f; //velocità di movimento ogni fram = 3px 

    int score =0 ; 
    
    // Main game loop - ogni iterazione è un frame 
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        // IsKeyDown  = true. significa che il quando il tasto passato in input è premuto fai qualcosa 
        if(IsKeyDown(KEY_RIGHT)) nextPos.x +=2; //destra 
        if(IsKeyDown(KEY_LEFT))  nextPos.x -=2; //sinestra 
        if(IsKeyDown(KEY_UP))    nextPos.y -=2; // su 
        if(IsKeyDown(KEY_DOWN))  nextPos.y +=2; //giu 
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        int mapCol = (int)(nextPos.x) / TILE_SIZE; 
        int mapRow = (int)(nextPos.y)/ TILE_SIZE; 
    
    // Fisica delle collisioni 
    if (mapRow >= 0 && mapRow < MAP_ROWS &&
        mapCol >= 0 && mapCol < MAP_COLS &&
        map[mapRow][mapCol] != '#') {
         pacmanPos = nextPos;

         int currentCol = (int)(pacmanPos.x)/TILE_SIZE; 

         int currentRow = (int)(pacmanPos.y)/TILE_SIZE; 

    
      //Quando pacman entra in una cella e trova il puntino e lo mangia 
    if(map[currentRow][currentCol] == '.')
    {
        map[currentRow][currentCol] =' '; 
        score +=10; 
    }
}
    
  
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
            DrawText(TextFormat("Score : %d", score),10,10,20,BLACK); 
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
