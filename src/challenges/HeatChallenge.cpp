#include <algorithm>
#include <cmath>
#include "challenges/HeatChallenge.h"
#include "game/Arena.h"
using namespace std;

namespace{
    const float CALOR_POR_SEG=0.25F; //4s moviendose sin parar para explotar
    const float ENFRIAR_POR_SEG=0.40F; //2.5s quieto para enfriarse del todo
    const float AVISO=0.70F; //a partir de aqui: alerta
    const float TRAS_EXPLOTAR=0.50F; //calor con el que vuelves tras explotar

    Color mezclar(Color a,Color b,float t){
        return Color{(unsigned char)(a.r+(b.r-a.r)*t),(unsigned char)(a.g+(b.g-a.g)*t),(unsigned char)(a.b+(b.b-a.b)*t),255};
    }

    Color colorCalor(float c){
        if(c<AVISO) return mezclar(YELLOW,ORANGE,c/AVISO);
        return mezclar(ORANGE,RED,(c-AVISO)/(1.0F-AVISO));
    }

    float pulso(float calor){
        if(calor<AVISO) return 1.0F;
        float k=(calor-AVISO)/(1.0F-AVISO);
        return 0.5F+0.5F*sin((float)GetTime()*(10.0F+12.0F*k));
    }
}

void HeatChallenge::update(float dt,bool activo,Context& ctx){
    if(activo && ctx.jugador.seMueve()) calor+=CALOR_POR_SEG*dt;
    else calor-=ENFRIAR_POR_SEG*dt;
    calor=clamp(calor,0.0F,1.0F);

    if(calor>=1.0F && !ctx.jugador.esInvulnerable()){
        ctx.jugador.recibirGolpe();
        calor=TRAS_EXPLOTAR;
    }
}

void HeatChallenge::dibujarMundo(const Context& ctx) const {
    if(ctx.jugador.estaMuerto() || calor<=0.01F) return;

    Rectangle a=Arena::area();
    Vector2 p=ctx.jugador.getPos();
    float r=ctx.jugador.getRadio();
    bool alerta=(calor>=AVISO);
    float pl=pulso(calor);
    Color c=colorCalor(calor);

    DrawRing(p,r+3,r+7,0.0F,360.0F,32,Fade(c,0.25F+0.5F*calor*pl));

    const float w=46.0F,h=7.0F;
    float by=p.y-r-20.0F;
    if(by<a.y+2) by=p.y+r+13.0F;
    Rectangle fondo={p.x-w/2,by,w,h};
    DrawRectangleRec(fondo,Fade(BLACK,0.55F));
    DrawRectangleRec({fondo.x,fondo.y,w*calor,h},Fade(c,alerta ? 0.5F+0.5F*pl : 1.0F));
    DrawRectangleLinesEx(fondo,1.0F,Fade(BLACK,0.8F));
}

void HeatChallenge::dibujarHud(const Context&) const {
    const int W=GetScreenWidth();
    const float w=W*0.11F;
    const float h=20.0F;
    const float x=W*0.64F-w/2;
    const float y=(Arena::HUD_ALTO-h)/2;
    bool alerta=(calor>=AVISO);
    float pl=pulso(calor);
    Color c=colorCalor(calor);

    const char* t="CALOR";
    DrawText(t,(int)x-MeasureText(t,16)-8,(int)(y+2),16,LIGHTGRAY);

    DrawRectangleRec({x,y,w,h},Fade(WHITE,0.15F));
    DrawRectangleRec({x,y,w*calor,h},Fade(c,alerta ? 0.55F+0.45F*pl : 1.0F));
    DrawRectangleLinesEx({x,y,w,h},2.0F,alerta ? Fade(RED,0.5F+0.5F*pl) : WHITE);
}