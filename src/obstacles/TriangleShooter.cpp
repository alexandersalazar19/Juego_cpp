#include "game/Theme.h"
#include <cmath>
#include "obstacles/TriangleShooter.h"
#include "obstacles/Laser.h"
#include "obstacles/Geometry.h"
#include "game/Arena.h"
using namespace std;

namespace{
    const float AVISO=0.9F; //segundos de parpadeo antes de disparar
    const float PARPADEO_MIN=3.0F; //parpadeos por segundo al empezar el aviso
    const float PARPADEO_MAX=16.0F; //y justo antes de disparar
}

TriangleShooter::TriangleShooter(Vector2 c,float r,Vector2 v,float g,float ca,float vl):Obstacle(25),centro(c),radio(r),vel(v),angulo((float)GetRandomValue(0,359)),giro(g),cada(ca),velLaser(vl),reloj(0.0F),fase(0.0F){}

void TriangleShooter::vertices(Vector2 v[3],Vector2 d) const {
    for(int i=0;i<3;i++){
        float a=(angulo+i*120.0F)*DEG2RAD;
        v[i]={centro.x+d.x+cos(a)*radio,centro.y+d.y+sin(a)*radio};
    }
}

bool TriangleShooter::dentroDelArea() const {
    Rectangle a=Arena::area();
    float m=radio+10.0F;
    return centro.x>a.x+m && centro.x<a.x+a.width-m && centro.y>a.y+m && centro.y<a.y+a.height-m;
}

bool TriangleShooter::avisando() const {
    return reloj>cada-AVISO;
}

void TriangleShooter::update(float dt){
    centro.x+=vel.x*dt;
    centro.y+=vel.y*dt;
    angulo+=giro*dt;
    reloj+=dt;

    if(avisando()){
        float urgencia=clamp(1.0F-(cada-reloj)/AVISO,0.0F,1.0F); //0 al empezar el aviso, 1 al disparar
        fase+=dt*(PARPADEO_MIN+(PARPADEO_MAX-PARPADEO_MIN)*urgencia);
    }

    if(reloj>=cada && dentroDelArea()){ //solo dispara cuando esta dentro de la pantalla
        for(int i=0;i<3;i++){
            float a=(angulo+i*120.0F)*DEG2RAD;
            Vector2 dir={cos(a),sin(a)};
            pendientes.push_back({{centro.x+dir.x*radio,centro.y+dir.y*radio},dir});
        }
        reloj=0.0F;
        fase=0.0F;
    }
}

void TriangleShooter::draw(float alfa) const {
    bool encendido=avisando() && fmod(fase,1.0F)>=0.5F; //la mitad del ciclo en rojo
    Color c=encendido ? Theme::LASER : Theme::TRIANGULO;

    Vector2 v[3];
    vertices(v,{0.0F,0.0F});
    DrawTriangle(v[0],v[2],v[1],Fade(c,alfa)); //orden antihorario
}

bool TriangleShooter::collidesWith(Vector2 cj,float r) const {
    Vector2 v[3];
    vertices(v,{0.0F,0.0F});
    if(CheckCollisionPointTriangle(cj,v[0],v[1],v[2])) return true; //el centro esta dentro
    for(int i=0;i<3;i++){
        if(distanciaASegmento(cj,v[i],v[(i+1)%3])<=r) return true; //roza un lado
    }
    return false;
}

bool TriangleShooter::isOffscreen() const {
    Rectangle a=Arena::area();
    if(vel.x>0 && centro.x-radio>a.x+a.width) return true;
    if(vel.x<0 && centro.x+radio<a.x) return true;
    if(vel.y>0 && centro.y-radio>a.y+a.height) return true;
    if(vel.y<0 && centro.y+radio<a.y) return true;
    return false;
}

void TriangleShooter::drawGhost(Vector2 d,Color relleno,Color borde) const {
    Vector2 v[3];
    vertices(v,d);
    DrawTriangle(v[0],v[2],v[1],relleno);
    for(int i=0;i<3;i++) DrawLineEx(v[i],v[(i+1)%3],2.0F,borde);
}

void TriangleShooter::emitir(vector<unique_ptr<Obstacle>>& out){
    for(const auto& d:pendientes) out.push_back(make_unique<Laser>(d.pos,d.dir,velLaser));
    pendientes.clear();
}

Rectangle TriangleShooter::getBounds() const {return {centro.x-radio,centro.y-radio,radio*2,radio*2};}
Color TriangleShooter::getColor() const {return Theme::TRIANGULO;}
