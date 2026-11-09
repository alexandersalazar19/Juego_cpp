#include <memory>
#include <vector>
#include <algorithm>
#include "raylib.h"
#include "entities/Player.h"
#include "entities/Coin.h"
#include "game/Spawner.h"
#include "game/Arena.h"
#include "game/Difficulty.h"
#include "game/Levels.h"
#include "game/Context.h"
#include "challenges/ChallengeManager.h"
#include "powerups/PowerUpManager.h"
#include "ui/Hud.h"
#include "game/SpawnQueue.h"
#include "effects/Effects.h"
#include "game/Theme.h"
using namespace std;

enum class Estado{Menu,Jugando,Muriendo,GameOver};

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
    ChallengeManager retos;
    PowerUpManager powerups;
    SpawnQueue cola;
    Effects efectos;

    Estado estado=Estado::Menu;
    int seleccion=0; //boton marcado en el menu
    int dificultad=0;
    int puntos=0;
    float temporizador=0.0F;
    float tiempoJugado=0.0F;
    float tMensaje=0.0F;

    auto nuevaPartida=[&](){
        obstaculos.clear();
        cola.clear();
        efectos.clear();
        Vector2 centro=Arena::centro();
        jugador=Player(centro.x,centro.y);
        moneda.respawn();
        puntos=0;
        temporizador=0.0F;
        tiempoJugado=0.0F;
        estado=Estado::Jugando;
        niveles.reiniciar(dificultad);
        const ConfigDificultad& c0=getConfig(dificultad);
        retos.configurar(niveles.getMods(),c0);
        powerups.reiniciar(c0);
    };

    while(!WindowShouldClose()){
        float dt=GetFrameTime();
        if(estado!=Estado::Menu) efectos.update(dt);
        const ConfigDificultad& cfg=getConfig(dificultad);
        int nivel=niveles.getNivel();
        Context ctx{jugador,cfg,nivel,puntos,efectos};

        switch(estado){
            case Estado::Menu:{
                Vector2 md=GetMouseDelta();
                int bajo=botonBajoMouse();
                if((md.x!=0 || md.y!=0) && bajo>=0) seleccion=bajo; //el raton solo manda cuando se mueve
                if(IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) seleccion=(seleccion+1)%NUM_DIFICULTADES;
                if(IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) seleccion=(seleccion+NUM_DIFICULTADES-1)%NUM_DIFICULTADES;

                int elegido=-1;
                if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) elegido=seleccion;
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && bajo>=0) elegido=bajo;
                for(int i=0;i<NUM_DIFICULTADES;i++){
                    if(IsKeyPressed(KEY_ONE+i)) elegido=i;
                }
                if(elegido>=0){
                    seleccion=elegido;
                    dificultad=elegido;
                    nuevaPartida();
                }
                break;
            }

            case Estado::Jugando:{
                int vidasAntes=jugador.getVidas();
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
                        ListaObstaculos nuevos;
                        spawnObstaculos(nuevos,velocidadActual(cfg,nivel,rampa),cfg);
                        cola.add(nuevos,avisoActual(cfg,nivel));
                    }
                }
                cola.update(dt,obstaculos);

                for(auto it=obstaculos.begin();it!=obstaculos.end();){
                    (*it)->update(dt);
                    if((*it)->collidesWith(jugador.getPos(),jugador.getRadio())) jugador.recibirGolpe();

                    if((*it)->isOffscreen()){
                        puntos+=puntosPor((*it)->getPoints(),cfg,nivel);
                        it=obstaculos.erase(it);
                    }else it++;
                }

                retos.update(dt,niveles.puedeSpawnear(),ctx);
                if(jugador.getVidas()<vidasAntes){
                    powerups.alRecibirGolpe();
                    efectos.golpe(jugador.getPos(),jugador.getRadio());
                }
                powerups.update(dt,niveles.puedeSpawnear(),ctx);

                if(niveles.update(dt,!obstaculos.empty() || !cola.empty())){
                    puntos+=puntosPor(500,cfg,niveles.getNivel()-1); //bonus por el nivel que acabas de superar
                    temporizador=0.0F;
                    retos.configurar(niveles.getMods(),cfg);
                    powerups.alCambiarNivel();
                }

                if(jugador.estaMuerto()){
                    estado=Estado::Muriendo;
                    efectos.iniciarMuerte(jugador.getPos(),jugador.getRadio());
                }
                break;
            }

            case Estado::Muriendo:{
                float dtM=dt*efectos.getEscalaTiempo(); //el mundo sigue, pero en camara lenta
                cola.update(dtM,obstaculos);
                for(auto& o:obstaculos) o->update(dtM);
                if(efectos.muerteTerminada()){
                    estado=Estado::GameOver;
                    tMensaje=0.0F;
                }
                break;
            }

            case Estado::GameOver:
                tMensaje+=dt;
                if(IsKeyPressed(KEY_R)) nuevaPartida();
                if(IsKeyPressed(KEY_M)){
                    seleccion=dificultad; //el menu vuelve marcando la que jugabas
                    estado=Estado::Menu;
                }
                break;
        }

        BeginDrawing();
            ClearBackground(Theme::FONDO);

            if(estado==Estado::Menu) dibujarMenu(seleccion);
            else{
                Rectangle a=Arena::area();
                Camera2D cam={efectos.getTemblor(),{0.0F,0.0F},0.0F,1.0F};
                BeginScissorMode((int)a.x,(int)a.y,(int)a.width,(int)a.height);
                    BeginMode2D(cam);
                        dibujarRejilla();
                        moneda.draw();
                        for(const auto& o:obstaculos) o->draw(retos.alfaObstaculo(o->distanciaA(jugador.getPos())));
                        if(estado==Estado::Jugando) jugador.draw();
                        retos.dibujarCapa(ctx); //oscuridad
                        retos.dibujarMundo(ctx); //bombas y barra de calor: se ven sobre la oscuridad
                        powerups.dibujar(jugador);
                    EndMode2D();
                    if(estado==Estado::Jugando){
                        retos.dibujarAlerta(ctx); //resplandor de la bomba
                        cola.draw(); //avisos de los bordes
                    }
                    efectos.dibujarOscuridad(); //oscuridad de la muerte
                    BeginMode2D(cam);
                        efectos.draw();
                    EndMode2D();
                EndScissorMode();

                dibujarHud(puntos,cfg.nombre,nivel,tiempoJugado);
                dibujarCargas(jugador);
                dibujarVidas(jugador);
                retos.dibujarHud(ctx);
                if(estado==Estado::Jugando && niveles.getFase()==FaseNivel::Anuncio) dibujarAnuncio(nivel,nombreMods(niveles.getMods()).c_str(),niveles.getProgresoAnuncio());

                if(estado==Estado::GameOver){
                    float alfa=min(1.0F,tMensaje/0.6F);
                    Vector2 m=Arena::centro();
                    const char* t1="PERDISTE";
                    const char* t2="R: reiniciar  M: menú";
                    DrawText(t1,(int)m.x-MeasureText(t1,60)/2,(int)m.y-60,60,Fade(Theme::PELIGRO,alfa));
                    DrawText(t2,(int)m.x-MeasureText(t2,24)/2,(int)m.y+10,24,Fade(Theme::TEXTO,alfa));
                }
            }
        EndDrawing();
    }
    retos.limpiar();
    CloseWindow();
    return 0;
}