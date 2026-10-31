#include "Coin.h"
#include "Arena.h"

Coin::Coin(float x,float y):pos{x,y},radio(12.0F){}

void Coin::draw() const {
    DrawCircleV(pos,radio,GOLD);
}

bool Coin::collidesWith(Vector2 centro,float r) const {
    return CheckCollisionCircles(pos,radio,centro,r);
}

void Coin::respawn(){
    const Rectangle a=Arena::area();
    const int margen=20;
    pos.x=(float)GetRandomValue((int)a.x+margen,(int)(a.x+a.width)-margen);
    pos.y=(float)GetRandomValue((int)a.y+margen,(int)(a.y+a.height)-margen);
}