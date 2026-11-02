#include <memory>
#include <vector>
#include "raylib.h"
#include "entities/Player.h"
#include "entities/Coin.h"
#include "game/Spawner.h"
#include "game/Arena.h"
#include "ui/Hud.h"
#include "game/Difficulty.h"
#include "entities/Pickup.h"
#include "game/Levels.h"
using namespace std;

enum class Estado{Menu,Jugando,GameOver};

float variar(float t){
    return t*(0.7F+GetRandomValue(0,60)/100.0F);
}

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
    ControlNiveles niveles;

    Estado estado=Estado::Menu;
    int dificultad=0;
    int puntos=0;
    float temporizador=0.0F;
    float tiempoJugado=0.0F;

    vector<Pickup> pickups;
    float proxBomba=0.0F;
    float proxVida=0.0F;
    float desafio=0.0F; //segundos que quedan del reto de vida

    auto nuevaPartida=[&](){
        obstaculos.clear();
        Vector2 centro=Arena::centro();
        jugador=Player(centro.x,centro.y);
        moneda.respawn();
        puntos=0;
        temporizador=0.0F;
        tiempoJugado=0.0F;
        estado=Estado::Jugando;
        pickups.clear();
        desafio=0.0F;
        niveles.reiniciar(dificultad);
        const ConfigDificultad& c0=getConfig(dificultad);
        proxBomba=variar(c0.bombaCada);
        proxVida=variar(c0.vidaCada);
    };

    while(!WindowShouldClose()){
        float dt=GetFrameTime();
        const ConfigDificultad& cfg=getConfig(dificultad);
        int nivel=niveles.getNivel();

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

                if(niveles.puedeSpawnear()){
                    temporizador+=dt;
                    float rampa=niveles.getRampa();
                    if(temporizador>=intervaloActual(cfg,nivel,rampa)){
                        temporizador=0.0F;
                        spawnObstaculos(obstaculos,velocidadActual(cfg,nivel,rampa),cfg);
                    }
                }

                for(auto it=obstaculos.begin();it!=obstaculos.end();){
                    (*it)->update(dt);
                    if((*it)->collidesWith(jugador.getPos(),jugador.getRadio()) && jugador.recibirGolpe()) desafio=0.0F;

                    if((*it)->isOffscreen()){
                        puntos+=puntosPor((*it)->getPoints(),cfg,nivel);
                        it=obstaculos.erase(it);
                    }else it++;
                }

                if(niveles.update(dt,!obstaculos.empty())){
                    puntos+=puntosPor(500,cfg,niveles.getNivel()-1); //bonus por el nivel que acabas de superar
                    temporizador=0.0F;
                    jugador.recargarCargas();
                    pickups.clear();
                    proxBomba=variar(cfg.bombaCada);
                }

                //aparicion de pickups
                //bombas
                if((niveles.getMods()&MOD_BOMBA) && niveles.puedeSpawnear()){
                    proxBomba-=dt;
                    if(proxBomba<=0){
                        pickups.push_back(crearPickup(P_BOMBA,jugador.getPos()));
                        proxBomba=variar(cfg.bombaCada);
                    }
                }
                //vidas
                if(cfg.vidaCada>0 && niveles.puedeSpawnear()){
                    proxVida-=dt;
                    if(proxVida<=0){
                        bool hayVida=false;
                        for(const auto& p:pickups) if(p.getTipo()==P_VIDA) hayVida=true;
                        if(!hayVida && desafio<=0 && jugador.getVidas()<jugador.getMaxVidas()) pickups.push_back(crearPickup(P_VIDA,jugador.getPos()));
                        proxVida=variar(cfg.vidaCada);
                    }
                }

                //recoger o caducar
                for(auto it=pickups.begin();it!=pickups.end();){
                    it->update(dt);
                    if(it->collidesWith(jugador.getPos(),jugador.getRadio())){
                        if(it->getTipo()==P_BOMBA) puntos+=puntosPor(150,cfg,nivel);
                        else desafio=TIEMPO_DESAFIO;
                        it=pickups.erase(it);
                    }else if(it->haExpirado()){
                        if(it->getTipo()==P_BOMBA && jugador.recibirGolpe()) desafio=0.0F;
                        it=pickups.erase(it);
                    }else it++;
                }

                //reto de vida
                if(desafio>0){
                    desafio-=dt;
                    if(desafio<=0){
                        desafio=0.0F;
                        if(!jugador.ganarVida()) puntos+=puntosPor(200,cfg,nivel); //ya tenia el maximo
                    }
                }
                if(jugador.estaMuerto()) estado=Estado::GameOver;
                break;

            case Estado::GameOver:
                if(IsKeyPressed(KEY_R)) nuevaPartida();
                if(IsKeyPressed(KEY_M)) estado=Estado::Menu;
                break;
        }

        float alerta=-1.0F; //-1=no hay bombas
        for(const auto& p:pickups){
            if(p.getTipo()==P_BOMBA) alerta=max(alerta,1.0F-p.getFraccion());
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
                    for(const auto& p:pickups) p.draw();
                    if(alerta>=0) dibujarAlertaBomba(alerta);
                    if(desafio>0) DrawRing(jugador.getPos(),jugador.getRadio()+5,jugador.getRadio()+8,-90.0F,-90.0F+360.0F*(desafio/TIEMPO_DESAFIO),32,GREEN);
                EndScissorMode();
    
                dibujarHud(puntos,cfg.nombre,nivel,tiempoJugado);
                dibujarCargas(jugador);
                dibujarVidas(jugador);
                if(niveles.getFase()==FaseNivel::Anuncio) dibujarAnuncio(nivel,nombreMods(niveles.getMods()).c_str(),niveles.getProgresoAnuncio());
    
                if(estado==Estado::GameOver){
                    Vector2 m=Arena::centro();
                    const char* t1="PERDISTE";
                    const char* t2="R: reiniciar  M: menú";
                    DrawText(t1,(int)m.x-MeasureText(t1,60)/2,(int)m.y-60,60,RED);
                    DrawText(t2,(int)m.x-MeasureText(t2,24)/2,(int)m.y+10,24,DARKGRAY);
                }
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}