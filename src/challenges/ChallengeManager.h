#pragma once
#include <memory>
#include <vector>
#include "challenges/Challenge.h"

class ChallengeManager{
public:
    void configurar(int mods,const ConfigDificultad& cfg);
    void update(float dt,bool activo,Context& ctx);
    void dibujarMundo() const;
    void dibujarCapa(const Context& ctx) const;

private:
    std::vector<std::unique_ptr<Challenge>> lista;
};