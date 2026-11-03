#pragma once
#include "challenges/Challenge.h"

class HeatChallenge:public Challenge{
public:
    void update(float dt,bool activo,Context& ctx) override;
    void dibujarMundo(const Context& ctx) const override;
    void dibujarHud(const Context& ctx) const override;

private:
    float calor=0.0F; //0.0 frio, 1.0 explota
};