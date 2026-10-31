#include <cmath>
#include "Spawner.h"
#include "Wall.h"
#include "CircleObstacle.h"
#include "DiagonalWall.h"
using namespace std;

namespace{
    const float GROSOR=24.0F;
    const float HUECO=120.0F;
    const float RADIO_CIRCULO=18.0F;

    void paredSimple(ListaObstaculos& out,int lado,float vel){
        const float W=(float)GetScreenWidth();
        const float H=(float)GetScreenHeight();
        const float largo=150.0F;
        float y=(float)GetRandomValue(0,(int)(H-largo));
        float x=(float)GetRandomValue(0,(int)(W-largo));

        switch(lado){
            case 0: out.push_back(make_unique<Wall>(Rectangle{-GROSOR,y,GROSOR,largo},Vector2{vel,0})); break;
            case 1: out.push_back(make_unique<Wall>(Rectangle{W,y,GROSOR,largo},Vector2{-vel,0})); break;
            case 2: out.push_back(make_unique<Wall>(Rectangle{x,-GROSOR,largo,GROSOR},Vector2{0,vel})); break;
            default: out.push_back(make_unique<Wall>(Rectangle{x,H,largo,GROSOR},Vector2{0,-vel})); break;
        }
    }

    void paredDoble(ListaObstaculos& out,int lado,float vel){
        const float W=(float)GetScreenWidth();
        const float H=(float)GetScreenHeight();

        bool vertical=(!lado || lado==1); //muros verticales entran por derecha o izquierda
        float largoLado=vertical ? H : W;

        float inicioHueco=(float)GetRandomValue(40,(int)(largoLado-HUECO-40));
        float finHueco=inicioHueco+HUECO;

        Rectangle r1,r2;
        Vector2 v;
        if(vertical){
            float x=(!lado) ? -GROSOR : W;
            v={(!lado) ? vel : -vel,0.0F};
            r1={x,0.0F,GROSOR,inicioHueco};
            r2={x,finHueco,GROSOR,largoLado-finHueco};
        }else{
            float y=(lado==2) ? -GROSOR : H;
            v={0.0F,(lado==2) ? vel : -vel};
            r1={0.0F,y,inicioHueco,GROSOR};
            r2={finHueco,y,largoLado-finHueco,GROSOR};
        }

        out.push_back(make_unique<Wall>(r1,v));
        out.push_back(make_unique<Wall>(r2,v));
    }

    void paredDiagonal(ListaObstaculos& out,int lado,float vel){
        const float W=(float)GetScreenWidth();
        const float H=(float)GetScreenHeight();
        const Vector2 tam={150.0F,GROSOR};
        const float R=0.5F*sqrt(tam.x*tam.x + tam.y*tam.y);

        float x=(float)GetRandomValue(0,(int)W);
        float y=(float)GetRandomValue(0,(int)H);

        //apunta al centro de la pantalla para que no se salga de inmediato
        Vector2 pos,dir;
        switch(lado){
            case 0: pos={-R,y}; dir={1,(y<H/2) ? 1.0F : -1.0F}; break;
            case 1: pos={W+R,y}; dir={-1,(y<H/2) ? 1.0F : -1.0F}; break;
            case 2: pos={x,-R}; dir={(x<W/2) ? 1.0F : -1.0F,1}; break;
            default: pos={x,H+R}; dir={(x<W/2) ? 1.0F : -1.0F,-1}; break;
        }

        Vector2 v={dir.x*vel*0.7071F,dir.y*vel*0.7071F};

        float anguloMov=atan2(v.y,v.x)*RAD2DEG;
        float angulo=anguloMov+90.0F+(float)GetRandomValue(-30,30);

        out.push_back(make_unique<DiagonalWall>(pos,tam,angulo,v));
    }

    void circuloOnda(ListaObstaculos& out,int lado,float vel){
        const float W=(float)GetScreenWidth();
        const float H=(float)GetScreenHeight();
        const float r=RADIO_CIRCULO;
        float x=(float)GetRandomValue(0,(int)W);
        float y=(float)GetRandomValue(0,(int)H);

        float amplitud=(float)GetRandomValue(40,120);
        float frecuencia=GetRandomValue(20,40)/10.0F; //entre 2.0 y 4.0 rad/s

        Vector2 pos,v;
        switch(lado){
            case 0: pos={-r,y}; v={vel,0}; break;
            case 1: pos={W+r,y}; v={-vel,0}; break;
            case 2: pos={x,-r}; v={0,vel}; break;
            default: pos={x,H+r}; v={0,-vel}; break;
        }
        out.push_back(make_unique<CircleObstacle>(pos,r,v,amplitud,frecuencia));
    }
}

void spawnObstaculos(ListaObstaculos& out,float velocidad){
    int lado=GetRandomValue(0,3); //0=izq, 1=der, 2=arriba, 3=abajo
    switch(GetRandomValue(0,6)){
        case 0:
        case 1:
        case 2:
            paredSimple(out,lado,velocidad); break;
        case 3: paredDoble(out,lado,velocidad); break;
        case 4: paredDiagonal(out,lado,velocidad); break;
        default: circuloOnda(out,lado,velocidad); break;
    }
}