#include "powerups/Pickup.h"
#include "game/Theme.h"

Pickup::Pickup(TipoPickup t,Vector2 p):tipo(t),pos(p),radio(14.0F),duracion(12.0F),restante(duracion){}

void Pickup::update(float dt){
    restante-=dt;
}

void Pickup::draw() const {
    float f=restante/duracion;
    Color c=Theme::POWERUP;
    if(f<0.3F && (int)(restante*8)%2==0) c=Fade(c,0.3F);
    DrawCircleV(pos,radio,c);
    DrawRing(pos,radio+4,radio+7,-90.0F,-90.0F+360.0F*f,32,Fade(Theme::POWERUP,0.9F));
    DrawRectangle((int)pos.x-2,(int)pos.y-8,4,16,Theme::HUD);
    DrawRectangle((int)pos.x-8,(int)pos.y-2,16,4,Theme::HUD);
}

bool Pickup::collidesWith(Vector2 centro,float r) const {
    return CheckCollisionCircles(pos,radio,centro,r);
}

bool Pickup::haExpirado() const {return restante<=0;}
TipoPickup Pickup::getTipo() const {return tipo;}