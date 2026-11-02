#pragma once
#include "raylib.h"

enum TipoPickup{
    P_BOMBA,
    P_VIDA
};

const float TIEMPO_DESAFIO=8.0F;

class Pickup{
public:
    Pickup(TipoPickup tipo,Vector2 pos);
    void update(float dt);
    void draw() const;
    bool collidesWith(Vector2 centro,float radio) const;
    bool haExpirado() const;
    float getFraccion() const;
    TipoPickup getTipo() const;

private:
    TipoPickup tipo;
    Vector2 pos;
    float radio;
    float duracion;
    float restante;
};