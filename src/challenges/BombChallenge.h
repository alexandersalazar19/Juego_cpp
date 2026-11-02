#pragma once
#include <vector>
#include "challenges/Challenge.h"

class BombChallenge:public Challenge{
public:
    BombChallenge(const ConfigDificultad& cfg);

    void update(float dt,bool activo,Context& ctx) override;
    void dibujarMundo() const override;
    void dibujarCapa(const Context& ctx) const override;

private:
    struct Bomba{
        Vector2 pos;
        float restante;
    };
    std::vector<Bomba> bombas;
    float cada; //segundos entre bombas
    float proxima;
};