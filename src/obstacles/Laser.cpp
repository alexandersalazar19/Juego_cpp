#include "game/Theme.h"
#include "obstacles/Laser.h"
#include "obstacles/Geometry.h"
#include "game/Arena.h"
using namespace std;

namespace{
    const float LARGO=95.0F;
    const float GROSOR=5.0F;
    const float VIDA_MAX=20.0F; //por si algo sale mal, nunca vive para siempre
}

Laser::Laser(Vector2 p,Vector2 d,float v):Obstacle(5),pos(p),dir(d),vel(v),edad(0.0F){}

Vector2 Laser::cola() const {
    return {pos.x-dir.x*LARGO,pos.y-dir.y*LARGO};
}

void Laser::update(float dt){
    pos.x+=dir.x*vel*dt;
    pos.y+=dir.y*vel*dt;
    edad+=dt;
}

void Laser::draw(float alfa) const {
    Vector2 c=cola();
    DrawLineEx(c,pos,GROSOR*3.0F,Fade(Theme::LASER,0.20F*alfa)); //halo
    DrawLineEx(c,pos,GROSOR,Fade(Theme::LASER,alfa));
    DrawLineEx(c,pos,GROSOR*0.4F,Fade(Theme::TEXTO,0.9F*alfa)); //nucleo brillante
    DrawCircleV(pos,GROSOR*0.9F,Fade(Theme::TEXTO,alfa)); //punta
}

bool Laser::collidesWith(Vector2 centro,float radio) const {
    return distanciaASegmento(centro,cola(),pos)<=radio+GROSOR*0.5F;
}

bool Laser::isOffscreen() const {
    Rectangle a=Arena::area();
    a.x-=20.0F;
    a.y-=20.0F;
    a.width+=40.0F;
    a.height+=40.0F;
    bool dentro=CheckCollisionPointRec(pos,a) || CheckCollisionPointRec(cola(),a);
    return !dentro || edad>VIDA_MAX;
}

void Laser::drawGhost(Vector2 d,Color relleno,Color) const {
    Vector2 c=cola();
    DrawLineEx({c.x+d.x,c.y+d.y},{pos.x+d.x,pos.y+d.y},GROSOR,relleno);
}

Rectangle Laser::getBounds() const {
    Vector2 c=cola();
    float x0=min(pos.x,c.x)-GROSOR;
    float y0=min(pos.y,c.y)-GROSOR;
    float x1=max(pos.x,c.x)+GROSOR;
    float y1=max(pos.y,c.y)+GROSOR;
    return {x0,y0,x1-x0,y1-y0};
}

Color Laser::getColor() const {return Theme::LASER;}