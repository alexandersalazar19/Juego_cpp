#include "raylib.h"
#include "Player.h"
using namespace std;

int main(){
    InitWindow(800,600,"Juego en C++");
    SetTargetFPS(60);

    Player jugador(400,300);

    while(!WindowShouldClose()){
        jugador.update(GetFrameTime());

        BeginDrawing();
            ClearBackground(RAYWHITE);
            jugador.draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}