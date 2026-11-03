#pragma once
#include "challenges/Challenge.h"

class DarkChallenge:public Challenge{
public:
    DarkChallenge();
    ~DarkChallenge() override;
    DarkChallenge(const DarkChallenge&)=delete;
    DarkChallenge& operator=(const DarkChallenge&)=delete;

    void update(float dt,bool activo,Context& ctx) override;
    void dibujarCapa(const Context& ctx) const override;

private:
    Texture2D luz;
    float entrada=0.0F;
    float salida=0.0F;
    bool huboActivo=false;
    float intensidad() const;
};