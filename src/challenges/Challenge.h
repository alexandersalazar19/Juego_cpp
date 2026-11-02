#pragma once
#include "game/Context.h"

class Challenge{
public:
    virtual ~Challenge()=default;

    //activo = el nivel esta en su fase activa (puede generar cosas)
    virtual void update(float dt,bool activo,Context& ctx)=0;
    virtual void dibujarMundo() const {}
    virtual void dibujarCapa(const Context&) const {}
};