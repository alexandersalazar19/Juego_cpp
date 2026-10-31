#include <cmath>
#include "Spawner.h"
#include "Wall.h"
#include "CircleObstacle.h"
#include "DiagonalWall.h"
#include "Arena.h"
using namespace std;

namespace{
    const float GROSOR_MIN=20.0F,GROSOR_MAX=40.0F;
    const float LARGO_MIN=120.0F,LARGO_MAX=360.0F;
    const float HUECO_MIN=100.0F,HUECO_MAX=180.0F;
    const float RADIO_MIN=15.0F,RADIO_MAX=23.0F;

    float aleatorio(float min,float max){
        return min+(max-min)*GetRandomValue(0,1000)/1000.0F;
    }

    void paredSimple(ListaObstaculos& out,int lado,float vel){
        const Rectangle a=Arena::area();
        const float grosor=aleatorio(GROSOR_MIN,GROSOR_MAX);
        const float largo=aleatorio(LARGO_MIN,LARGO_MAX);


        float y=(float)GetRandomValue((int)a.x,(int)(a.x+a.width-largo));
        float x=(float)GetRandomValue((int)a.y,(int)(a.y+a.height-largo));
        float der=a.x+a.width;
        float abajo=a.y+a.height;

        switch(lado){
            case 0: out.push_back(make_unique<Wall>(Rectangle{a.x-grosor,y,grosor,largo},Vector2{vel,0})); break;
            case 1: out.push_back(make_unique<Wall>(Rectangle{der,y,grosor,largo},Vector2{-vel,0})); break;
            case 2: out.push_back(make_unique<Wall>(Rectangle{x,a.y-grosor,largo,grosor},Vector2{0,vel})); break;
            default: out.push_back(make_unique<Wall>(Rectangle{x,abajo,largo,grosor},Vector2{0,-vel})); break;
        }
    }

    void paredDoble(ListaObstaculos& out,int lado,float vel){
        const Rectangle a=Arena::area();
        const float grosor=aleatorio(GROSOR_MIN,GROSOR_MAX);
        const float hueco=aleatorio(HUECO_MIN,HUECO_MAX);

        bool vertical=(!lado || lado==1);
        float largoLado=vertical ? a.height : a.width;

        float inicioHueco=(float)GetRandomValue(40,(int)(largoLado-hueco-40));
        float finHueco=inicioHueco+hueco;

        Rectangle r1,r2;
        Vector2 v;
        if(vertical){
            float x=(!lado) ? a.x-grosor : a.x+a.width;
            v={(!lado) ? vel : -vel,0.0F};
            r1={x,a.y,grosor,inicioHueco};
            r2={x,a.y+finHueco,grosor,largoLado-finHueco};
        }else{
            float y=(lado==2) ? a.y-grosor : a.y+a.height;
            v={0.0F,(lado==2) ? vel : -vel};
            r1={a.x,y,inicioHueco,grosor};
            r2={a.x+finHueco,y,largoLado-finHueco,grosor};
        }

        out.push_back(make_unique<Wall>(r1,v));
        out.push_back(make_unique<Wall>(r2,v));
    }

    void paredDiagonal(ListaObstaculos& out,int lado,float vel){
        const Rectangle a=Arena::area();
        const Vector2 tam={aleatorio(LARGO_MIN,LARGO_MAX),aleatorio(GROSOR_MIN,GROSOR_MAX)};
        const float R=0.5F*sqrt(tam.x*tam.x + tam.y*tam.y);

        float x=(float)GetRandomValue((int)a.x,(int)(a.x+a.width));
        float y=(float)GetRandomValue((int)a.y,(int)(a.y+a.height));
        float midX=a.x+a.width/2;
        float midY=a.y+a.height/2;

        //apunta al centro de la pantalla para que no se salga de inmediato
        Vector2 pos,dir;
        switch(lado){
            case 0: pos={a.x-R,y}; dir={1,(y<midY) ? 1.0F : -1.0F}; break;
            case 1: pos={a.x+a.width+R,y}; dir={-1,(y<midY) ? 1.0F : -1.0F}; break;
            case 2: pos={x,a.y-R}; dir={(x<midX) ? 1.0F : -1.0F,1}; break;
            default: pos={x,a.y+a.height+R}; dir={(x<midX) ? 1.0F : -1.0F,-1}; break;
        }

        Vector2 v={dir.x*vel*0.7071F,dir.y*vel*0.7071F};

        float anguloMov=atan2(v.y,v.x)*RAD2DEG;
        float angulo=anguloMov+90.0F+(float)GetRandomValue(-30,30);

        out.push_back(make_unique<DiagonalWall>(pos,tam,angulo,v));
    }

    void circuloOnda(ListaObstaculos& out,int lado,float vel){
        const Rectangle a=Arena::area();
        const float r=aleatorio(RADIO_MIN,RADIO_MAX);
        float x=(float)GetRandomValue((int)a.x,(int)(a.x+a.width));
        float y=(float)GetRandomValue((int)a.y,(int)(a.y+a.height));

        float amplitud=(float)GetRandomValue(40,120);
        float frecuencia=GetRandomValue(20,40)/10.0F; //entre 2.0 y 4.0 rad/s

        Vector2 pos,v;
        switch(lado){
            case 0: pos={a.x-r,y}; v={vel,0}; break;
            case 1: pos={a.x+a.width+r,y}; v={-vel,0}; break;
            case 2: pos={x,a.y-r}; v={0,vel}; break;
            default: pos={x,a.y+a.height+r}; v={0,-vel}; break;
        }
        out.push_back(make_unique<CircleObstacle>(pos,r,v,amplitud,frecuencia));
    }
}

void spawnObstaculos(ListaObstaculos& out,float velocidad){
    int lado=GetRandomValue(0,3); //0=izq, 1=der, 2=arriba, 3=abajo
    float vel=velocidad*aleatorio(0.8F,1.3F);
    switch(GetRandomValue(0,6)){
        case 0:
        case 1:
        case 2:
            paredSimple(out,lado,vel); break;
        case 3: paredDoble(out,lado,vel); break;
        case 4: paredDiagonal(out,lado,vel); break;
        default: circuloOnda(out,lado,vel); break;
    }
}