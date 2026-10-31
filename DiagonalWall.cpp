#include <cmath>
#include "DiagonalWall.h"
using namespace std;

DiagonalWall::DiagonalWall(Vector2 c,Vector2 t,float a,Vector2 v):Obstacle(20),centro(c),tam(t),angulo(a),vel(v),radioEnvolvente(0.5F*sqrt(t.x*t.x + t.y*t.y)){}

void DiagonalWall::update(float dt){
    centro.x+=vel.x*dt;
    centro.y+=vel.y*dt;
}

void DiagonalWall::draw() const {
    //el rectangulo gira alrededor de un origen que se pone en su centro
    DrawRectanglePro(Rectangle{centro.x,centro.y,tam.x,tam.y},Vector2{tam.x/2,tam.y/2},angulo,GRAY);
}

bool DiagonalWall::collidesWith(Vector2 cj,float radio) const {
    float rad=-angulo*DEG2RAD;
    float dx=cj.x-centro.x;
    float dy=cj.y-centro.y;

    Vector2 local={dx*cos(rad) - dy*sin(rad),dx*sin(rad) + dy*cos(rad)};

    Rectangle rectLocal={-tam.x/2,-tam.y/2,tam.x,tam.y};
    return CheckCollisionCircleRec(local,radio,rectLocal);
}

bool DiagonalWall::isOffscreen() const {
    float R=radioEnvolvente;
    if(vel.x>0 && centro.x-R>GetScreenWidth()) return true;
    if(vel.x<0 && centro.x+R<0) return true;
    if(vel.y>0 && centro.y-R>GetScreenHeight()) return true;
    if(vel.y<0 && centro.y+R<0) return true;
    return false;
}