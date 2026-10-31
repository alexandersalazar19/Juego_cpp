#pragma once
#include "raylib.h"

class Player{
public:
    Player(float x,float t);

    void update(float dt);
    void draw() const;

    Vector2 getPos() const;
    float getRadio() const;

    bool esInvulnerable() const;
    int getCargas() const;
    int getMaxCargas() const;
    float getProgresoRecarga() const;

private:
    Vector2 pos;
    float radio;
    float velocidad;

    int cargas;
    float recarga;
    float invul;
    Vector2 destinoTp() const;
};