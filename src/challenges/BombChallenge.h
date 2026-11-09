#pragma once
#include <vector>
#include "challenges/Challenge.h"
using namespace std;

class BombChallenge:public Challenge{
public:
    BombChallenge(const ConfigDificultad& cfg);

    void update(float dt,bool activo,Context& ctx) override;
    void dibujarMundo(const Context& ctx) const override;
    void dibujarAlerta(const Context& ctx) const override;

private:
    struct Bomba{
        Vector2 pos;
        float restante;
    };

    struct Explosion{ //onda expansiva en camino: dana al jugador cuando lo alcanza
        Vector2 pos;
        float t;
        bool golpeo;
    };
    
    vector<Bomba> bombas;
    vector<Explosion> explosiones;
    float cada; //segundos entre bombas
    float proxima;
};