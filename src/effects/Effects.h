#pragma once
#include <vector>
#include "raylib.h"
using namespace std;

class Effects{
public:
    void golpe(Vector2 pos,float radioJugador);
    void iniciarMuerte(Vector2 pos,float radioJugador);
    void update(float dtReal);
    void draw() const;
    void dibujarOscuridad() const;
    Vector2 getTemblor() const;
    float getEscalaTiempo() const;
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
    };
    struct Onda{
        Vector2 pos;
        float t;
        float radioIni;
        float radioMax;
        float duracion;
        Color color;
    };
    vector<Particula> particulas;
    vector<Onda> ondas;

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