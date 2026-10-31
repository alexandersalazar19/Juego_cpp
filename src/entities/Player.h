#pragma once
#include "raylib.h"

class Player{
public:
    Player(float x,float t);

    void update(float dt);
    void draw() const;

    Vector2 getPos() const;
    float getRadio() const;

private:
    Vector2 pos;
    float radio;
    float velocidad;
};