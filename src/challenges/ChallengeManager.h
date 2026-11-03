#pragma once
#include <memory>
#include <vector>
#include "challenges/Challenge.h"
using namespace std;

class ChallengeManager{
public:
    void configurar(int mods,const ConfigDificultad& cfg);
    void limpiar();
    void update(float dt,bool activo,Context& ctx);
    void dibujarCapa(const Context& ctx) const;
    void dibujarMundo(const Context& ctx) const;
    void dibujarAlerta(const Context& ctx) const;
    void dibujarHud(const Context& ctx) const;
    float alfaObstaculo(float distancia) const;

private:
    vector<unique_ptr<Challenge>> lista;
};