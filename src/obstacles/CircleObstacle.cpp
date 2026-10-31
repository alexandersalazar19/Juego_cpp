#include <cmath>
#include "obstacles/CircleObstacle.h"
#include "game/Arena.h"
using namespace std;

CircleObstacle::CircleObstacle(Vector2 p,float r,Vector2 v,float amp,float frec):Obstacle(15),base(p),pos(p),radio(r),vel(v),perp{0.0F,0.0F},amplitud(amp),frecuencia(frec),tiempo(0.0F){
    float largo=sqrt(vel.x*vel.x + vel.y*vel.y);
    if(largo>0) perp={-vel.y/largo,vel.x/largo};
}

void CircleObstacle::update(float dt){
    tiempo+=dt;
    base.x+=vel.x*dt;
    base.y+=vel.y*dt;

    float desvio=amplitud*sin(frecuencia*tiempo);
    pos.x=base.x+perp.x*desvio;
    pos.y=base.y+perp.y*desvio;
}

void CircleObstacle::draw() const {
    DrawCircleV(pos,radio,DARKGRAY);
}

bool CircleObstacle::collidesWith(Vector2 centro,float r) const {
    return CheckCollisionCircles(pos,radio,centro,r);
}

bool CircleObstacle::isOffscreen() const {
    Rectangle a=Arena::area();
    if(vel.x>0 && pos.x-radio>a.x+a.width) return true;
    if(vel.x<0 && pos.x+radio<a.x) return true;
    if(vel.y>0 && pos.y-radio>a.y+a.height) return true;
    if(vel.y<0 && pos.y+radio<a.y) return true;
    return false;
}