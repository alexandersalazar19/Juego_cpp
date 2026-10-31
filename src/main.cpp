#include <memory>
#include <vector>
#include "raylib.h"
#include "entities/Player.h"
#include "entities/Coin.h"
#include "game/Spawner.h"
#include "game/Arena.h"
#include "ui/Hud.h"
#include "game/Difficulty.h"
using namespace std;

enum class Estado{Menu,Jugando,GameOver};

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

    Estado estado=Estado::Menu;
    int dificultad=0;
    int puntos=0;
    float temporizador=0.0F;
    float tiempoJugado=0.0F;

    auto nuevaPartida=[&](){
        obstaculos.clear();
        Vector2 centro=Arena::centro();
        jugador=Player(centro.x,centro.y);
        moneda.respawn();
        puntos=0;
        temporizador=0.0F;
        tiempoJugado=0.0F;
        estado=Estado::Jugando;
    };

    while(!WindowShouldClose()){
        float dt=GetFrameTime();
        const ConfigDificultad& cfg=getConfig(dificultad);
        int nivel=nivelPorTiempo(tiempoJugado);

        switch(estado){
            case Estado::Menu:
                for(int i=0;i<NUM_DIFICULTADES;i++){
                    if(IsKeyPressed(KEY_ONE+i)){
                        dificultad=i;
                        nuevaPartida();
                    }
                }
                break;

            case Estado::Jugando:
                tiempoJugado+=dt;
                jugador.update(dt);

                if(moneda.collidesWith(jugador.getPos(),jugador.getRadio())){
                    puntos+=puntosPor(50,cfg,nivel);
                    moneda.respawn();
                }

                temporizador+=dt;
                if(temporizador>=intervaloActual(cfg,nivel)){
                    temporizador=0.0F;
                    spawnObstaculos(obstaculos,velocidadActual(cfg,nivel),cfg);
                }

                for(auto it=obstaculos.begin();it!=obstaculos.end();){
                    (*it)->update(dt);
                    if((*it)->collidesWith(jugador.getPos(),jugador.getRadio())) estado=Estado::GameOver;

                    if((*it)->isOffscreen()){
                        puntos+=puntosPor((*it)->getPoints(),cfg,nivel);
                        it=obstaculos.erase(it);
                    }else it++;
                }
                break;

            case Estado::GameOver:
                if(IsKeyPressed(KEY_R)) nuevaPartida();
                if(IsKeyPressed(KEY_M)) estado=Estado::Menu;
                break;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            if(estado==Estado::Menu) dibujarMenu();
            else{
                Rectangle a=Arena::area();
                BeginScissorMode((int)a.x,(int)a.y,(int)a.width,(int)a.height);
                    moneda.draw();
                    for(const auto& o:obstaculos) o->draw();
                    jugador.draw();
                EndScissorMode();
    
                dibujarHud(puntos,cfg.nombre,nivel,tiempoJugado);
    
                if(estado==Estado::GameOver){
                    Vector2 m=Arena::centro();
                    const char* t1="GAME OVER";
                    const char* t2="R: restart  M: menu";
                    DrawText(t1,(int)m.x-MeasureText(t1,60)/2,(int)m.y-60,60,RED);
                    DrawText(t2,(int)m.x-MeasureText(t2,24)/2,(int)m.y+10,24,DARKGRAY);
                }
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}