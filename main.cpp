#include "raylib.h"
#include "Player.h"
#include "Coin.h"

int main(){
    InitWindow(800,600,"Juego en C++");
    SetTargetFPS(60);

    Player jugador(400,300);
    Coin moneda(200,200);
    int puntos=0;

    while(!WindowShouldClose()){
        jugador.update(GetFrameTime());

        if(moneda.collidesWith(jugador.getPos(),jugador.getRadio())){
            puntos+=10;
            moneda.respawn();
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            jugador.draw();
            moneda.draw();
            DrawText(TextFormat("Puntaje: %d",puntos),10,10,24,DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}