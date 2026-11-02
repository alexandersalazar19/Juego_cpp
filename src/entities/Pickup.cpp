#include <cmath>
#include "entities/Pickup.h"
using namespace std;

Pickup::Pickup(TipoPickup t,Vector2 p):tipo(t),pos(p),radio(t==P_BOMBA ? 18.0F : 14.0F),duracion(t==P_BOMBA ? 6.0F : 12.0F),restante(duracion){}

void Pickup::update(float dt){
    restante-=dt;
}

void Pickup::draw() const {
    float f=restante/duracion;

    if(tipo==P_BOMBA){
        bool alerta=(f<0.4F && (int)(restante*10)%2==0);
        DrawCircleV(pos,radio,alerta ? RED : BLACK);
        DrawCircleLinesV(pos,radio,DARKGRAY);
        DrawCircleV({pos.x-radio*0.35F,pos.y-radio*0.35F},radio*0.2F,Fade(WHITE,0.35F)); //brillo

        Vector2 base={pos.x+radio*0.5F,pos.y-radio*0.85F}; //mecha
        Vector2 punta={pos.x+radio*1.0F,pos.y-radio*1.5F};
        DrawLineEx(base,punta,3.0F,BROWN);
        float s=0.6F+0.4F*sin(restante*30.0F); //chispa que titila
        DrawCircleV(punta,3.0F+3.0F*s,ORANGE);
        DrawCircleV(punta,2.0F,YELLOW);

        DrawRing(pos,radio+4,radio+7,-90.0F,-90.0F+360.0F*f,32,RED);
        const char* t=TextFormat("%d",(int)ceil(restante));
        DrawText(t,(int)pos.x-MeasureText(t,20)/2,(int)pos.y-10,20,WHITE);
        return;
    }

    Color c=GREEN;
    if(f<0.3F && (int)(restante*8)%2==0) c=Fade(c,0.3F);
    DrawCircleV(pos,radio,c);
    DrawRing(pos,radio+4,radio+7,-90.0F,-90.0F+360.0F*f,32,DARKGREEN);
    DrawRectangle((int)pos.x-2,(int)pos.y-8,4,16,WHITE); //cruz
    DrawRectangle((int)pos.x-8,(int)pos.y-2,16,4,WHITE);
}

bool Pickup::collidesWith(Vector2 centro,float r) const {
    return CheckCollisionCircles(pos,radio,centro,r);
}

bool Pickup::haExpirado() const{
    return restante<=0;
}

float Pickup::getFraccion() const {return restante/duracion;}
TipoPickup Pickup::getTipo() const {return tipo;}