#include <algorithm>
#include <cmath>
#include "effects/Effects.h"
#include "game/Arena.h"
using namespace std;

namespace{
    const int N_PARTICULAS=26;
    const float DURACION_ONDA=0.40F;
    const float RADIO_ONDA=110.0F;

    const float T_EXPLOSION=1.1F;
    const float T_TOTAL=2.4F;
    const float ESCALA_MIN=0.15F;
    const float ESCALA_FINAL=0.5F;
    const float OSCURIDAD_MAX=0.6F;
    const float DURACION_FLASH=0.15F;
}

void Effects::sacudir(float duracion,float amplitud){
    temblor=duracion;
    duracionTemblor=duracion;
    amplitudTemblor=amplitud;
}

void Effects::golpe(Vector2 pos,float radioJugador){
    const Color colores[3]={MAROON,RED,DARKGRAY};
    for(int i=0;i<N_PARTICULAS;i++){
        float ang=GetRandomValue(0,359)*DEG2RAD;
        float rapidez=(float)GetRandomValue(150,450);
        float vida=GetRandomValue(40,80)/100.0F;
        particulas.push_back({pos,{cos(ang)*rapidez,sin(ang)*rapidez},vida,vida,(float)GetRandomValue(3,7),colores[i%3]});
    }
    ondas.push_back({pos,0.0F,radioJugador,RADIO_ONDA,DURACION_ONDA,BLACK});
    sacudir(0.30F,9.0F);
}

void Effects::iniciarMuerte(Vector2 pos,float radioJugador){
    muriendo=true;
    exploto=false;
    tMuerte=0.0F;
    posMuerte=pos;
    radioMuerte=radioJugador;
}

void Effects::explotar(){
    exploto=true;
    const Color colores[4]={RED,ORANGE,GOLD,MAROON};
    for(int i=0;i<90;i++){
        float ang=GetRandomValue(0,359)*DEG2RAD;
        float rapidez=(float)GetRandomValue(120,720);
        float vida=GetRandomValue(40,90)/100.0F;
        particulas.push_back({posMuerte,{cos(ang)*rapidez,sin(ang)*rapidez},vida,vida,(float)GetRandomValue(4,12),colores[i%4]});
    }
    ondas.push_back({posMuerte,0.0F,radioMuerte,300.0F,0.9F,ORANGE});
    ondas.push_back({posMuerte,0.0F,radioMuerte,180.0F,0.6F,RED});
    sacudir(0.5F,16.0F);
    flash=DURACION_FLASH;
}

void Effects::update(float dtReal){
    if(muriendo){
        tMuerte+=dtReal;
        if(!exploto && tMuerte>=T_EXPLOSION) explotar();
    }
    float dt=dtReal*getEscalaTiempo();

    for(auto it=particulas.begin();it!=particulas.end();){
        it->vida-=dt;
        if(it->vida<=0){
            it=particulas.erase(it);
            continue;
        }
        it->pos.x+=it->vel.x*dt;
        it->pos.y+=it->vel.y*dt;
        float roce=max(0.0F,1.0F-4.0F*dt);
        it->vel.x*=roce;
        it->vel.y*=roce;
        it++;
    }

    for(auto it=ondas.begin();it!=ondas.end();){
        it->t+=dt;
        if(it->t>=it->duracion) it=ondas.erase(it);
        else it++;
    }

    if(temblor>0) temblor=max(0.0F,temblor-dtReal);
    if(flash>0) flash=max(0.0F,flash-dtReal);
}

void Effects::draw() const {
    for(const auto& o:ondas){
        float f=o.t/o.duracion;
        float e=1.0F-(1.0F-f)*(1.0F-f);
        float r=o.radioIni+(o.radioMax-o.radioIni)*e;
        float grosor=1.0F+6.0F*(1.0F-f);
        DrawRing(o.pos,max(0.0F,r-grosor),r,0.0F,360.0F,48,Fade(o.color,0.8F*(1.0F-f)));
    }
    for(const auto& p:particulas){
        float f=p.vida/p.total;
        DrawCircleV(p.pos,p.tam*(0.4F+0.6F*f),Fade(p.color,f));
    }

    if(muriendo && !exploto){
        float p=tMuerte/T_EXPLOSION;
        float vib=3.0F*p;
        Vector2 c={posMuerte.x+GetRandomValue(-100,100)/100.0F*vib,posMuerte.y+GetRandomValue(-100,100)/100.0F*vib};
        Color col=((int)(tMuerte*20)%2) ? RED : MAROON;
        DrawCircleV(c,radioMuerte*(1.0F+0.6F*p*p),col);
    }
}

void Effects::dibujarOscuridad() const {
    Rectangle a=Arena::area();
    if(muriendo){
        float f=min(1.0F,tMuerte/T_TOTAL);
        DrawRectangleRec(a,Fade(BLACK,OSCURIDAD_MAX*f));
    }
    if(flash>0) DrawRectangleRec(a,Fade(GOLD,0.5F*flash/DURACION_FLASH));
}

Vector2 Effects::getTemblor() const {
    if(temblor<=0) return {0.0F,0.0F};
    float k=temblor/duracionTemblor;
    float amp=amplitudTemblor*k*k;
    return {GetRandomValue(-100,100)/100.0F*amp,GetRandomValue(-100,100)/100.0F*amp};
}

float Effects::getEscalaTiempo() const {
    if(!muriendo) return 1.0F;
    if(tMuerte<T_EXPLOSION){
        float f=tMuerte/T_EXPLOSION;
        float e=1.0F-(1.0F-f)*(1.0F-f);
        return 1.0F-(1.0F-ESCALA_MIN)*e;
    }
    float f=min(1.0F,(tMuerte-T_EXPLOSION)/(T_TOTAL-T_EXPLOSION));
    return ESCALA_MIN+(ESCALA_FINAL-ESCALA_MIN)*f;
}

bool Effects::muerteTerminada() const {
    return muriendo && tMuerte>=T_TOTAL;
}

void Effects::clear(){
    particulas.clear();
    ondas.clear();
    temblor=0.0F;
    flash=0.0F;
    muriendo=false;
    exploto=false;
    tMuerte=0.0F;
}