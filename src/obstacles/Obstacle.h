#pragma once
#include "raylib.h"

class Obstacle{
public:
    virtual ~Obstacle()=default;

    virtual void update(float dt)=0;
    virtual void draw() const=0;
    virtual bool collidesWith(Vector2 centro,float radio) const=0;
    virtual bool isOffscreen() const=0;

    int getPoints() const {return points;}

protected:
    explicit Obstacle(int pts):points(pts){}
    int points;
};