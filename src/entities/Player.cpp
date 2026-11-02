#include <cmath>
#include <algorithm>
#include "entities/Player.h"
#include "game/Arena.h"
using namespace std;

namespace{
    const int MAX_CARGAS=3;
    const float TIEMPO_RECARGA=6.0F;
    const float ALCANCE_TP=0.0F;
    const float INVUL_TP=0.0F;
    const int MAX_VIDAS=4;
    const int VIDAS_INICIALES=3;
    const float INVUL_GOLPE=1.5F;
}

Player::Player(float x,float y):pos{x,y},radio(14.0F),velocidad(340.0F),cargas(MAX_CARGAS),recarga(0.0F),invul(0.0F),vidas(VIDAS_INICIALES){}

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

    if(invul>0) invul-=dt;
    if(cargas<MAX_CARGAS){
        recarga+=dt;
        if(recarga>=TIEMPO_RECARGA){
            cargas++;
            recarga=0.0F;
        }
    }
    if(cargas>0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(),Arena::area())){
        pos=destinoTp();
        cargas--;
        invul=INVUL_TP;
    }
}

void Player::draw() const {
    Color c=MAROON;
    if(invul>0 && !((int)(invul*10)%2)) c=Fade(MAROON,0.35F); //parpadeo
    if(cargas>0 && CheckCollisionPointRec(GetMousePosition(),Arena::area())){
        if(ALCANCE_TP>0) DrawCircleLinesV(pos,ALCANCE_TP,Fade(GRAY,0.5F));
        DrawCircleLinesV(destinoTp(),radio,Fade(MAROON,0.6F));
    }
    DrawCircleV(pos,radio,c);
}

bool Player::esInvulnerable() const {
    return invul>0;
}

void Player::recargarCargas(){
    cargas=MAX_CARGAS;
    recarga=0.0F;
}

Vector2 Player::destinoTp() const {
    Rectangle a=Arena::area();
    Vector2 m=GetMousePosition();
    if(ALCANCE_TP>0){
        float dx=m.x-pos.x;
        float dy=m.y-pos.y;
        float d=std::sqrt(dx*dx+dy*dy);
        if(d>ALCANCE_TP){
            m.x=pos.x+dx/d*ALCANCE_TP;
            m.y=pos.y+dy/d*ALCANCE_TP;
        }
    }
    m.x=std::clamp(m.x,a.x+radio,a.x+a.width-radio);
    m.y=std::clamp(m.y,a.y+radio,a.y+a.height-radio);
    return m;
}

bool Player::recibirGolpe(){
    if(invul>0) return false;
    vidas--;
    invul=INVUL_GOLPE;
    return true;
}

bool Player::estaMuerto() const {
    return vidas<=0;
}

bool Player::ganarVida(){
    if(vidas>=MAX_VIDAS) return false;
    vidas++;
    return true;
}

Vector2 Player::getPos() const {return pos;}
float Player::getRadio() const {return radio;}
int Player::getCargas() const {return cargas;}
int Player::getMaxCargas() const {return MAX_CARGAS;}
float Player::getProgresoRecarga() const {return (cargas>=MAX_CARGAS) ? 1.0F : recarga/TIEMPO_RECARGA;}
int Player::getVidas() const {return vidas;}
int Player::getMaxVidas() const {return MAX_VIDAS;}