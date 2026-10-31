#include "Player.h"
#include <cmath>

Player::Player(float x,float y):pos{x,y},radio(20.0F),velocidad(300.0F){}

void Player::update(float dt){
    Vector2 dir={0.0F,0.0F};
    if(IsKeyDown(KEY_D)) dir.x++;
    if(IsKeyDown(KEY_A)) dir.x--;
    if(IsKeyDown(KEY_S)) dir.y++;
    if(IsKeyDown(KEY_W)) dir.y--;

    //Hacer que no se mueva mas rapido en diagonal
    float largo=sqrt(dir.x*dir.x + dir.y*dir.y);
    if(largo>0){
        dir.x/=largo;
        dir.y/=largo;
    }

    pos.x+=dir.x*velocidad*dt;
    pos.y+=dir.y*velocidad*dt;

    //Limites de la ventana
    if(pos.x<radio) pos.x=radio;
    if(pos.y<radio) pos.y=radio;
    if(pos.x>GetScreenWidth()-radio) pos.x=GetScreenWidth()-radio;
    if(pos.y>GetScreenHeight()-radio) pos.y=GetScreenHeight()-radio;
}

void Player::draw() const {
    DrawCircleV(pos,radio,MAROON);
}

Vector2 Player::getPos() const {return pos;}