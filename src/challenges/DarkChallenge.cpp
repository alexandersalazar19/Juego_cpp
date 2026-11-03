#include <algorithm>
#include <cmath>
#include "challenges/DarkChallenge.h"
#include "game/Arena.h"
using namespace std;

namespace{
    const float RADIO_INICIAL=950.0F;
    const float RADIO_FINAL=300.0F;
    const float NUCLEO=0.10F;
    const float OSCURIDAD=0.92F;
    const float DURACION_ENTRADA=4.0F;
    const float DURACION_SALIDA=7.5F;
    const float MARGEN=40.0F;

    float suave(float t){
        t=clamp(t,0.0F,1.0F);
        return t*t*(3.0F-2.0F*t);
    }
}

DarkChallenge::DarkChallenge(){
    const int N=256;
    Image img=GenImageColor(N,N,BLANK);
    for(int y=0;y<N;y++){
        for(int x=0;x<N;x++){
            float dx=(x+0.5F-N/2.0F)/(N/2.0F);
            float dy=(y+0.5F-N/2.0F)/(N/2.0F);
            float d=sqrt(dx*dx+dy*dy);
            float a=suave((d-NUCLEO)/(1.0F-NUCLEO));
            ImageDrawPixel(&img,x,y,Color{0,0,0,(unsigned char)(a*255.0F)});
        }
    }
    luz=LoadTextureFromImage(img);
    UnloadImage(img);
    SetTextureFilter(luz,TEXTURE_FILTER_BILINEAR);
}

DarkChallenge::~DarkChallenge(){
    UnloadTexture(luz);
}

void DarkChallenge::update(float dt,bool activo,Context&){
    if(activo){
        huboActivo=true;
        entrada+=dt;
    }else if(huboActivo) salida+=dt;
}

float DarkChallenge::intensidad() const {
    float fin=suave(entrada/DURACION_ENTRADA);
    float fout=1.0F-suave(salida/DURACION_SALIDA);
    return fin*fout;
}

void DarkChallenge::dibujarCapa(const Context& ctx) const {
    float k=intensidad();
    if(k<=0.001F) return;

    float radio=RADIO_INICIAL+(RADIO_FINAL-RADIO_INICIAL)*k;
    float alfa=OSCURIDAD*k;

    Rectangle a=Arena::area();
    Vector2 p=ctx.jugador.getPos();
    Color negro=Fade(BLACK,alfa);

    float x0=p.x-radio,x1=p.x+radio;
    float y0=p.y-radio,y1=p.y+radio;
    DrawTexturePro(luz,Rectangle{0,0,(float)luz.width,(float)luz.height},Rectangle{x0,y0,radio*2,radio*2},Vector2{0,0},0.0F,Fade(WHITE,alfa));

    float ax0=a.x-MARGEN,ay0=a.y-MARGEN;
    float ax1=a.x+a.width+MARGEN,ay1=a.y+a.height+MARGEN;
    float cx0=clamp(x0,ax0,ax1),cx1=clamp(x1,ax0,ax1);
    float cy0=clamp(y0,ay0,ay1),cy1=clamp(y1,ay0,ay1);
    DrawRectangleRec({ax0,ay0,ax1-ax0,cy0-ay0},negro);
    DrawRectangleRec({ax0,cy1,ax1-ax0,ay1-cy1},negro);
    DrawRectangleRec({ax0,cy0,cx0-ax0,cy1-cy0},negro);
    DrawRectangleRec({cx1,cy0,ax1-cx1,cy1-cy0},negro);
}