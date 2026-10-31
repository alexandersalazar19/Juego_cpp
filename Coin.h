#pragma once
#include "raylib.h"

class Coin{
public:
    Coin(float x,float y);

    void draw() const;
    bool collidesWith(Vector2 centro,float radio) const;
    void respawn();

private:
    Vector2 pos;
    float radio;
};