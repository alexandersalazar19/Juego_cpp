#include <algorithm>
#include <cmath>
#include "game/SpawnQueue.h"
#include "game/Arena.h"
using namespace std;

void SpawnQueue::add(ListaObstaculos& nuevos,float retraso){
    for(auto& o:nuevos) lista.push_back(Pendiente{move(o),retraso,retraso});
    nuevos.clear();
}

void SpawnQueue::update(float dt,ListaObstaculos& activos){
    for(auto it=lista.begin();it!=lista.end();){
        it->restante-=dt;
        if(it->restante<=0){
            activos.push_back(move(it->obs));
            it=lista.erase(it);
        }else it++;
    }
}

void SpawnQueue::draw() const {
    const Rectangle a=Arena::area();
    const float ax2=a.x+a.width;
    const float ay2=a.y+a.height;
    const float prof=30.0F; //grosor de la banda
    const float tol=1.0F;
    const float asomo=120.0F; //cuanto de la silueta se muestra dentro del area

    for(const auto& p:lista){
        Rectangle b=p.obs->getBounds();
        float urgencia=1.0F-p.restante/p.total; //0 al empezar, 1 al salir
        float pulso=0.5F+0.5F*sin((float)GetTime()*(8.0F+14.0F*urgencia));
        Color col=Fade(ORANGE,0.55F+0.4F*pulso);
        Color borde=Fade(RED,0.9F);

        //tramo del borde que ocupara (recortado al area)
        float x0=max(b.x,a.x),x1=min(b.x+b.width,ax2);
        float y0=max(b.y,a.y),y1=min(b.y+b.height,ay2);

        Rectangle r;
        Vector2 flecha,desp;
        float rot;
        if(b.x+b.width<=a.x+tol){ //entra por la izquierda
            r={a.x,y0,prof,y1-y0};
            flecha={a.x+prof+22.0F,(y0+y1)/2};
            rot=0.0F;
            desp={(a.x-(b.x+b.width))+min(b.width,asomo),0.0F};
        }else if(b.x>=ax2-tol){ //derecha
            r={ax2-prof,y0,prof,y1-y0};
            flecha={ax2-prof-22.0F,(y0+y1)/2};
            rot=180.0F;
            desp={-((b.x-ax2)+min(b.width,asomo)),0.0F};
        }else if(b.y+b.height<=a.y+tol){ //arriba
            r={x0,a.y,x1-x0,prof};
            flecha={(x0+x1)/2,a.y+prof+22.0F};
            rot=90.0F;
            desp={0.0F,(a.y-(b.y+b.height))+min(b.height,asomo)};
        }else{ //abajo
            r={x0,ay2-prof,x1-x0,prof};
            flecha={(x0+x1)/2,ay2-prof-22.0F};
            rot=270.0F;
            desp={0.0F,-((b.y-ay2)+min(b.height,asomo))};
        }

        p.obs->drawGhost(desp,Fade(ORANGE,0.25F+0.2F*pulso),borde); //forma real
    }
}

bool SpawnQueue::empty() const {return lista.empty();}
void SpawnQueue::clear(){lista.clear();}