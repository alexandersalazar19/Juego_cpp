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

    vector<Bomba> bombas;
    float cada; //segundos entre bombas
    float proxima;
};