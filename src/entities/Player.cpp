#include <cmath>
#include <algorithm>
#include "entities/Player.h"
#include "game/Arena.h"
using namespace std;

Player::Player(float x,float y):pos{x,y},radio(14.0F),velocidad(340.0F){}

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
    Rectangle a=Arena::area();
    pos.x=clamp(pos.x,a.x+radio,a.x+a.width-radio);
    pos.y=clamp(pos.y,a.y+radio,a.y+a.height-radio);
}

void Player::draw() const {
    DrawCircleV(pos,radio,MAROON);
}

Vector2 Player::getPos() const {return pos;}
float Player::getRadio() const {return radio;}