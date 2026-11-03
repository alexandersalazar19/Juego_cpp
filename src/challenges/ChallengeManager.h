#pragma once
#include <memory>
#include <vector>
#include "challenges/Challenge.h"

class ChallengeManager{
public:
    void configurar(int mods,const ConfigDificultad& cfg);
    void limpiar();
    void update(float dt,bool activo,Context& ctx);
    void dibujarCapa(const Context& ctx) const;
    void dibujarMundo() const;
    void dibujarAlerta(const Context& ctx) const;
    float alfaObstaculo(float distancia) const;

private:
    std::vector<std::unique_ptr<Challenge>> lista;
};