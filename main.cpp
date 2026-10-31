#include <memory>
#include <vector>
#include "raylib.h"
#include "Player.h"
#include "Coin.h"
#include "Spawner.h"
using namespace std;

int main(){
    InitWindow(800,600,"Juego en C++");
    SetTargetFPS(60);

    Player jugador(400,300);
    Coin moneda(200,200);
    vector<unique_ptr<Obstacle>> obstaculos;

    int puntos=0;
    bool gameOver=false;
    float temporizador=0.0F;
    const float intervalo=1.2F;
    const float velocidad=120.0F;

    while(!WindowShouldClose()){
        float dt=GetFrameTime();

        if(!gameOver){
            jugador.update(GetFrameTime());
    
            if(moneda.collidesWith(jugador.getPos(),jugador.getRadio())){
                puntos+=50;
                moneda.respawn();
            }

            temporizador+=dt;
            if(temporizador>=intervalo){
                temporizador=0.0F;
                spawnObstaculos(obstaculos,velocidad);
            }

            for(auto it=obstaculos.begin();it!=obstaculos.end();){
                (*it)->update(dt);
                if((*it)->collidesWith(jugador.getPos(),jugador.getRadio())) gameOver=true;
                if((*it)->isOffscreen()){
                    puntos+=(*it)->getPoints();
                    it=obstaculos.erase(it);
                }else it++;
            }
        }else if(IsKeyPressed(KEY_R)){
            obstaculos.clear();
            jugador=Player(400,300);
            puntos=0;
            temporizador=0.0F;
            gameOver=false;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            moneda.draw();
            for(const auto& o:obstaculos) o->draw();
            jugador.draw();
            DrawText(TextFormat("Points: %d",puntos),10,10,24,DARKGRAY);
            if(gameOver){
                DrawText("GAME OVER",270,250,50,RED);
                DrawText("Press R to restart",270,310,24,DARKGRAY);
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}