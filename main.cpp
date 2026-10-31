#include <memory>
#include <vector>
#include "raylib.h"
#include "Player.h"
#include "Coin.h"
#include "Spawner.h"
#include "Arena.h"
#include "Hud.h"
using namespace std;

int main(){
    InitWindow(800,600,"Juego en C++");

    int monitor=GetCurrentMonitor();
    SetWindowSize(GetMonitorWidth(monitor),GetMonitorHeight(monitor));
    ToggleFullscreen();

    SetTargetFPS(60);

    Vector2 c=Arena::centro();
    Player jugador(c.x,c.y);
    Coin moneda(200,200);
    vector<unique_ptr<Obstacle>> obstaculos;

    int puntos=0;
    bool gameOver=false;
    float temporizador=0.0F;
    float tiempoJugado=0.0F;
    const float intervalo=1.0F;
    const float velocidad=210.0F;

    while(!WindowShouldClose()){
        float dt=GetFrameTime();

        if(!gameOver){
            tiempoJugado+=dt;
            jugador.update(dt);
    
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
            Vector2 centro=Arena::centro();
            jugador=Player(centro.x,centro.y);
            puntos=0;
            temporizador=0.0F;
            tiempoJugado=0.0F;
            gameOver=false;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            Rectangle a=Arena::area();
            BeginScissorMode((int)a.x,(int)a.y,(int)a.width,(int)a.height);
                moneda.draw();
                for(const auto& o:obstaculos) o->draw();
                jugador.draw();
            EndScissorMode();

            dibujarHud(puntos,"Normal",1,tiempoJugado);

            if(gameOver){
                Vector2 m=Arena::centro();
                const char* t1="GAME OVER";
                const char* t2="Press R to restart";
                DrawText(t1,(int)m.x-MeasureText(t1,60)/2,(int)m.y-60,60,RED);
                DrawText(t2,(int)m.x-MeasureText(t2,24)/2,(int)m.y+10,24,DARKGRAY);
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}