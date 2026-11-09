#pragma once
#include <vector>
#include "raylib.h"
using namespace std;

constexpr float VELOCIDAD_ONDA_BOMBA=4500.0F; //px/s a la que viaja la onda de una bomba

class Effects{
public:
    void golpe(Vector2 pos,float radioJugador); //golpe normal: particulas, onda y temblor
    void explosion(Vector2 pos); //bomba que explota: bola de fuego, chispas y onda expansiva
    static float alcanceOnda(Vector2 pos); //distancia a la que la onda ya cubrio toda la zona de juego
    void iniciarMuerte(Vector2 pos,float radioJugador);
    void update(float dtReal);
    void draw() const; //todo lo que va dentro de la camara
    void dibujarOscuridad() const; //oscurecimiento y destello (sobre el area, sin camara)
    Vector2 getTemblor() const; //desplazamiento de camara en este frame
    float getEscalaTiempo() const; //1.0 normal, baja durante la muerte
    bool muerteTerminada() const;
    void clear();

private:
    struct Particula{
        Vector2 pos;
        Vector2 vel;
        float vida;
        float total;
        float tam;
        Color color;
        bool chispa; //true = se dibuja como estela alargada
    };

    struct Onda{
        Vector2 pos;
        float t;
        float radioIni;
        float radioMax;
        float duracion;
        Color color;
    };

    struct Destello{ //bola de fuego
        Vector2 pos;
        float t;
        float duracion;
        float radioMax;
    };

    struct OndaChoque{ //onda expansiva que cruza la pantalla
        Vector2 pos;
        float t;
        float radioMax;
    };

    vector<Particula> particulas;
    vector<Onda> ondas;
    vector<Destello> destellos;
    vector<OndaChoque> choques;

    float temblor=0.0F;
    float duracionTemblor=0.3F;
    float amplitudTemblor=0.0F;
    float flash=0.0F;

    bool muriendo=false;
    bool exploto=false;
    float tMuerte=0.0F;
    Vector2 posMuerte={0.0F,0.0F};
    float radioMuerte=0.0F;

    void sacudir(float duracion,float amplitud);
    void explotar();
};