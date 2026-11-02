#include "obstacles/Wall.h"
#include "game/Arena.h"

Wall::Wall(Rectangle r,Vector2 v):Obstacle(10),rect(r),vel(v){}

void Wall::update(float dt){
    rect.x+=vel.x*dt;
    rect.y+=vel.y*dt;
}

void Wall::draw() const {
    DrawRectangleRec(rect,GRAY);
}

bool Wall::collidesWith(Vector2 centro,float radio) const {
    return CheckCollisionCircleRec(centro,radio,rect);
}

bool Wall::isOffscreen() const {
    Rectangle a=Arena::area();
    if(vel.x>0 && rect.x>a.x+a.width) return true;
    if(vel.x<0 && rect.x+rect.width<a.x) return true;
    if(vel.y>0 && rect.y>a.y+a.height) return true;
    if(vel.y<0 && rect.y+rect.height<a.y) return true;
    return false;
}

void Wall::drawGhost(Vector2 d,Color relleno,Color borde) const {
    Rectangle r={rect.x+d.x,rect.y+d.y,rect.width,rect.height};
    DrawRectangleRec(r,relleno);
    DrawRectangleLinesEx(r,2.0F,borde);
}

Rectangle Wall::getBounds() const {return rect;}