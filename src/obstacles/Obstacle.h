#pragma once
#include <algorithm>
#include <cmath>
#include "raylib.h"
using namespace std;

class Obstacle{
public:
    virtual ~Obstacle()=default;

    virtual void update(float dt)=0;
    virtual void draw(float alfa) const=0;
    virtual bool collidesWith(Vector2 centro,float radio) const=0;
    virtual bool isOffscreen() const=0;
    virtual Rectangle getBounds() const=0;
    virtual Color getColor() const=0; //color propio segun su tipo
    virtual void drawGhost(Vector2 desplazamiento,Color relleno,Color borde) const=0; //silueta para el aviso

    int getPoints() const {return points;}

    float distanciaA(Vector2 p) const {
        Rectangle b=getBounds();
        float dx=max({b.x-p.x,0.0F,p.x-(b.x+b.width)});
        float dy=max({b.y-p.y,0.0F,p.y-(b.y+b.height)});
        return sqrt(dx*dx+dy*dy);
    }

protected:
    explicit Obstacle(int pts):points(pts){}
    int points;
};