#pragma once
#include "game/Context.h"

class Challenge{
public:
    virtual ~Challenge()=default;

    virtual void update(float dt,bool activo,Context& ctx)=0;
    virtual void dibujarMundo() const {}
    virtual void dibujarCapa(const Context&) const {}
    virtual float alfaObstaculo(float) const {return 1.0F;}
};