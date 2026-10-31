#pragma once
#include "Obstacle.h"

class Wall:public Obstacle{
public:
    Wall(Rectangle rect,Vector2 vel);

    void update(float dt) override;
    void draw() const override;
    bool collidesWith(Vector2 centro,float radio) const override;
    bool isOffscreen() const override;

private:
    Rectangle rect;
    Vector2 vel;
};