#include "Wall.h"

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
    if(vel.x>0 && rect.x>GetScreenWidth()) return true;
    if(vel.x<0 && rect.x+rect.width<0) return true;
    if(vel.y>0 && rect.y>GetScreenHeight()) return true;
    if(vel.y<0 && rect.y+rect.height<0) return true;
    return false;
}