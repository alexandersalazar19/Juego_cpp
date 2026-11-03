#pragma once
#include "game/Context.h"

class Challenge{
public:
    virtual ~Challenge()=default;

    virtual void update(float dt,bool activo,Context& ctx)=0;
    virtual void dibujarCapa(const Context&) const {}
    virtual void dibujarMundo() const {}
    virtual void dibujarAlerta(const Context&) const {}
    virtual float alfaObstaculo(float) const {return 1.0F;}
};