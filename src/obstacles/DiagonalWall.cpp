#include <cmath>
#include "obstacles/DiagonalWall.h"
#include "game/Arena.h"
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
    Rectangle a=Arena::area();
    float R=radioEnvolvente;
    if(vel.x>0 && centro.x-R>a.x+a.width) return true;
    if(vel.x<0 && centro.x+R<a.x) return true;
    if(vel.y>0 && centro.y-R>a.y+a.height) return true;
    if(vel.y<0 && centro.y+R<a.y) return true;
    return false;
}

void DiagonalWall::drawGhost(Vector2 d,Color relleno,Color borde) const {
    Vector2 c={centro.x+d.x,centro.y+d.y};
    DrawRectanglePro(Rectangle{c.x,c.y,tam.x,tam.y},Vector2{tam.x/2,tam.y/2},angulo,relleno);

    float rad=angulo*DEG2RAD;
    float cs=cos(rad);
    float sn=sin(rad);
    const float sx[4]={-1,1,1,-1};
    const float sy[4]={-1,-1,1,1};
    Vector2 p[4];
    for(int i=0;i<4;i++){
        float lx=sx[i]*tam.x/2;
        float ly=sy[i]*tam.y/2;
        p[i]={c.x+lx*cs-ly*sn,c.y+lx*sn+ly*cs};
    }
    for(int i=0;i<4;i++) DrawLineEx(p[i],p[(i+1)%4],2.0F,borde);
}
Rectangle DiagonalWall::getBounds() const {
    float rad=angulo*DEG2RAD;
    float c=fabs(cos(rad));
    float s=fabs(sin(rad));
    float hx=tam.x/2*c+tam.y/2*s;
    float hy=tam.x/2*s+tam.y/2*c;
    return {centro.x-hx,centro.y-hy,hx*2,hy*2};
}