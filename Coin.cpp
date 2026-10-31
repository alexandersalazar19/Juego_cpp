#include "Coin.h"

Coin::Coin(float x,float y):pos{x,y},radio(12.0F){}

void Coin::draw() const {
    DrawCircleV(pos,radio,GOLD);
}

bool Coin::collidesWith(Vector2 centro,float r) const {
    return CheckCollisionCircles(pos,radio,centro,r);
}

void Coin::respawn(){
    pos.x=(float)GetRandomValue(20,GetScreenWidth()-20);
    pos.y=(float)GetRandomValue(20,GetScreenHeight()-20);
}