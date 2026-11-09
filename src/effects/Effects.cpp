#include <algorithm>
#include <cmath>
#include "effects/Effects.h"
#include "game/Arena.h"
#include "game/Theme.h"
using namespace std;

namespace{
    //golpe normal
    const int N_PARTICULAS=26;
    const float DURACION_ONDA=0.40F;
    const float RADIO_ONDA=110.0F;

    //explosion de bomba
    const float RADIO_BOLA=120.0F; //tamano de la bola de fuego
    const float DURACION_BOLA=0.35F;
    const int N_CHISPAS=36;
    const int N_BRASAS=16;

    //secuencia de muerte (segundos reales)
    const float T_EXPLOSION=1.1F; //cuando explota la bolita
    const float T_TOTAL=2.4F; //cuando aparece el mensaje
    const float ESCALA_MIN=0.15F; //velocidad minima del juego (en la explosion)
    const float ESCALA_FINAL=0.5F; //velocidad con la que termina
    const float OSCURIDAD_MAX=0.78F;
    const float DURACION_FLASH=0.15F;
}

void Effects::sacudir(float duracion,float amplitud){
    temblor=duracion;
    duracionTemblor=duracion;
    amplitudTemblor=amplitud;
}

float Effects::alcanceOnda(Vector2 pos){
    Rectangle a=Arena::area();
    float dx=max(pos.x-a.x,a.x+a.width-pos.x); //hasta la esquina mas lejana
    float dy=max(pos.y-a.y,a.y+a.height-pos.y);
    return sqrt(dx*dx+dy*dy)+80.0F;
}

void Effects::golpe(Vector2 pos,float radioJugador){
    const Color colores[3]={Theme::JUGADOR,Theme::TEXTO,Theme::CIRCULO};
    for(int i=0;i<N_PARTICULAS;i++){
        float ang=GetRandomValue(0,359)*DEG2RAD;
        float rapidez=(float)GetRandomValue(150,450);
        float vida=GetRandomValue(40,80)/100.0F;
        particulas.push_back({pos,{cos(ang)*rapidez,sin(ang)*rapidez},vida,vida,(float)GetRandomValue(3,7),colores[i%3],false});
    }
    ondas.push_back({pos,0.0F,radioJugador,RADIO_ONDA,DURACION_ONDA,Theme::TEXTO});
    sacudir(0.30F,9.0F);
}

void Effects::explosion(Vector2 pos){
    destellos.push_back({pos,0.0F,DURACION_BOLA,RADIO_BOLA});
    choques.push_back({pos,0.0F,alcanceOnda(pos)});

    //chispas alargadas
    const Color chispas[3]={Theme::MONEDA,Theme::AVISO,Theme::TEXTO};
    for(int i=0;i<N_CHISPAS;i++){
        float ang=GetRandomValue(0,359)*DEG2RAD;
        float rapidez=(float)GetRandomValue(300,900);
        float vida=GetRandomValue(30,70)/100.0F;
        particulas.push_back({pos,{cos(ang)*rapidez,sin(ang)*rapidez},vida,vida,(float)GetRandomValue(2,4),chispas[i%3],true});
    }

    //brasas que flotan un poco mas
    const Color brasas[2]={Theme::PELIGRO,Theme::AVISO};
    for(int i=0;i<N_BRASAS;i++){
        float ang=GetRandomValue(0,359)*DEG2RAD;
        float rapidez=(float)GetRandomValue(60,280);
        float vida=GetRandomValue(60,120)/100.0F;
        particulas.push_back({pos,{cos(ang)*rapidez,sin(ang)*rapidez},vida,vida,(float)GetRandomValue(3,6),brasas[i%2],false});
    }

    sacudir(0.5F,10.0F);
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
    const Color colores[4]={Theme::PELIGRO,Theme::AVISO,Theme::MONEDA,Theme::TEXTO};
    for(int i=0;i<90;i++){
        float ang=GetRandomValue(0,359)*DEG2RAD;
        float rapidez=(float)GetRandomValue(120,720);
        float vida=GetRandomValue(40,90)/100.0F;
        particulas.push_back({posMuerte,{cos(ang)*rapidez,sin(ang)*rapidez},vida,vida,(float)GetRandomValue(4,12),colores[i%4],false});
    }
    ondas.push_back({posMuerte,0.0F,radioMuerte,300.0F,0.9F,Theme::AVISO});
    ondas.push_back({posMuerte,0.0F,radioMuerte,180.0F,0.6F,Theme::PELIGRO});
    sacudir(0.5F,16.0F);
    flash=DURACION_FLASH;
}

void Effects::update(float dtReal){
    if(muriendo){
        tMuerte+=dtReal;
        if(!exploto && tMuerte>=T_EXPLOSION) explotar();
    }
    float dt=dtReal*getEscalaTiempo(); //particulas y ondas van en camara lenta

    for(auto it=particulas.begin();it!=particulas.end();){
        it->vida-=dt;
        if(it->vida<=0){
            it=particulas.erase(it);
            continue;
        }
        it->pos.x+=it->vel.x*dt;
        it->pos.y+=it->vel.y*dt;
        float roce=max(0.0F,1.0F-4.0F*dt); //frena con el tiempo
        it->vel.x*=roce;
        it->vel.y*=roce;
        it++;
    }

    for(auto it=ondas.begin();it!=ondas.end();){
        it->t+=dt;
        if(it->t>=it->duracion) it=ondas.erase(it);
        else it++;
    }

    for(auto it=destellos.begin();it!=destellos.end();){
        it->t+=dt;
        if(it->t>=it->duracion) it=destellos.erase(it);
        else it++;
    }

    for(auto it=choques.begin();it!=choques.end();){
        it->t+=dt;
        if(VELOCIDAD_ONDA_BOMBA*it->t>=it->radioMax) it=choques.erase(it);
        else it++;
    }

    if(temblor>0) temblor=max(0.0F,temblor-dtReal); //temblor y destello en tiempo real
    if(flash>0) flash=max(0.0F,flash-dtReal);
}

void Effects::draw() const {
    //onda expansiva: frente brillante, estela que se adelgaza detras y halo delante
    for(const auto& c:choques){
        float r=VELOCIDAD_ONDA_BOMBA*c.t;
        float f=min(1.0F,r/c.radioMax); //0 al nacer, 1 al cubrir toda la pantalla
        float g=1.0F-f;
        float a=g*sqrt(g); //se desvanece mientras avanza
        float cola=60.0F+140.0F*g;
        DrawRing(c.pos,max(0.0F,r-cola),r,0.0F,360.0F,128,Fade(Theme::PELIGRO,0.10F*a));
        DrawRing(c.pos,max(0.0F,r-cola*0.5F),r,0.0F,360.0F,128,Fade(Theme::AVISO,0.16F*a));
        DrawRing(c.pos,max(0.0F,r-cola*0.2F),r,0.0F,360.0F,128,Fade(Theme::MONEDA,0.30F*a));
        DrawRing(c.pos,r,r+14.0F,0.0F,360.0F,128,Fade(Theme::AVISO,0.12F*a)); //halo delante del frente
        DrawRing(c.pos,max(0.0F,r-4.0F),r,0.0F,360.0F,128,Fade(Theme::TEXTO,0.95F*a)); //borde brillante
    }

    //bola de fuego: capas de mas grande y rojo a mas pequeno y blanco
    for(const auto& d:destellos){
        float f=d.t/d.duracion;
        float e=1.0F-(1.0F-f)*(1.0F-f);
        float r=d.radioMax*(0.3F+0.7F*e);
        float a=1.0F-f;
        DrawCircleV(d.pos,r,Fade(Theme::PELIGRO,0.25F*a));
        DrawCircleV(d.pos,r*0.75F,Fade(Theme::AVISO,0.35F*a));
        DrawCircleV(d.pos,r*0.50F,Fade(Theme::MONEDA,0.55F*a));
        DrawCircleV(d.pos,r*0.28F,Fade(Theme::TEXTO,0.95F*a));
    }

    for(const auto& o:ondas){
        float f=o.t/o.duracion; //0 al nacer, 1 al terminar
        float e=1.0F-(1.0F-f)*(1.0F-f); //sale rapido y frena
        float r=o.radioIni+(o.radioMax-o.radioIni)*e;
        float grosor=1.0F+6.0F*(1.0F-f);
        DrawRing(o.pos,max(0.0F,r-grosor),r,0.0F,360.0F,48,Fade(o.color,0.8F*(1.0F-f)));
    }

    for(const auto& p:particulas){
        float f=p.vida/p.total; //1 al nacer, 0 al morir
        if(p.chispa){
            Vector2 cola={p.pos.x-p.vel.x*0.04F,p.pos.y-p.vel.y*0.04F}; //estela contra la direccion de vuelo
            DrawLineEx(cola,p.pos,p.tam*f+1.0F,Fade(p.color,f));
        }else{
            DrawCircleV(p.pos,p.tam*(0.4F+0.6F*f),Fade(p.color,f));
        }
    }

    //la bolita se hincha y vibra antes de explotar
    if(muriendo && !exploto){
        float p=tMuerte/T_EXPLOSION;
        float vib=3.0F*p;
        Vector2 c={posMuerte.x+GetRandomValue(-100,100)/100.0F*vib,posMuerte.y+GetRandomValue(-100,100)/100.0F*vib};
        Color col=((int)(tMuerte*20)%2) ? Theme::PELIGRO : Theme::JUGADOR;
        DrawCircleV(c,radioMuerte*(1.0F+0.6F*p*p),col);
    }
}

void Effects::dibujarOscuridad() const {
    Rectangle a=Arena::area();
    if(muriendo){
        float f=min(1.0F,tMuerte/T_TOTAL);
        DrawRectangleRec(a,Fade(Theme::VELO,OSCURIDAD_MAX*f));
    }
    if(flash>0) DrawRectangleRec(a,Fade(Theme::TEXTO,0.5F*flash/DURACION_FLASH));
}

Vector2 Effects::getTemblor() const {
    if(temblor<=0) return {0.0F,0.0F};
    float k=temblor/duracionTemblor;
    float amp=amplitudTemblor*k*k; //cae rapido
    return {GetRandomValue(-100,100)/100.0F*amp,GetRandomValue(-100,100)/100.0F*amp};
}

float Effects::getEscalaTiempo() const {
    if(!muriendo) return 1.0F;
    if(tMuerte<T_EXPLOSION){ //baja rapido hasta el minimo
        float f=tMuerte/T_EXPLOSION;
        float e=1.0F-(1.0F-f)*(1.0F-f);
        return 1.0F-(1.0F-ESCALA_MIN)*e;
    }
    float f=min(1.0F,(tMuerte-T_EXPLOSION)/(T_TOTAL-T_EXPLOSION)); //y se recupera algo
    return ESCALA_MIN+(ESCALA_FINAL-ESCALA_MIN)*f;
}

bool Effects::muerteTerminada() const {
    return muriendo && tMuerte>=T_TOTAL;
}

void Effects::clear(){
    particulas.clear();
    ondas.clear();
    destellos.clear();
    choques.clear();
    temblor=0.0F;
    flash=0.0F;
    muriendo=false;
    exploto=false;
    tMuerte=0.0F;
}