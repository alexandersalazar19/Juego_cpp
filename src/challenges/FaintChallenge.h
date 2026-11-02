#pragma once
#include "challenges/Challenge.h"

class FaintChallenge:public Challenge{
public:
    void update(float dt,bool activo,Context& ctx) override;
    float alfaObstaculo(float distancia) const override;
};