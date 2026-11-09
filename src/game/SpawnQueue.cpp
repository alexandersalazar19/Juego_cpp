#include "game/Theme.h"
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
    const float tol=1.0F;
    const float asomo=120.0F; //cuanto de la silueta se muestra dentro del area

    for(const auto& p:lista){
        Rectangle b=p.obs->getBounds();
        float urgencia=1.0F-p.restante/p.total; //0 al empezar, 1 al salir
        float pulso=0.5F+0.5F*sin((float)GetTime()*(8.0F+14.0F*urgencia));
        Color c=p.obs->getColor();

        Vector2 desp;
        if(b.x+b.width<=a.x+tol) desp={(a.x-(b.x+b.width))+min(b.width,asomo),0.0F}; //izquierda
        else if(b.x>=ax2-tol) desp={-((b.x-ax2)+min(b.width,asomo)),0.0F}; //derecha
        else if(b.y+b.height<=a.y+tol) desp={0.0F,(a.y-(b.y+b.height))+min(b.height,asomo)}; //arriba
        else desp={0.0F,-((b.y-ay2)+min(b.height,asomo))}; //abajo

        //relleno del color de su tipo, contorno de aviso
        p.obs->drawGhost(desp,Fade(c,0.30F+0.25F*pulso),Fade(Theme::AVISO,0.95F));
    }
}

bool SpawnQueue::empty() const {return lista.empty();}
void SpawnQueue::clear(){lista.clear();}