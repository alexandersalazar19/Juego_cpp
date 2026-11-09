#pragma once
#include "obstacles/Obstacle.h"

class Wall:public Obstacle{
public:
    Wall(Rectangle rect,Vector2 vel);

    void update(float dt) override;
    void draw(float alfa) const override;
    bool collidesWith(Vector2 centro,float radio) const override;
    bool isOffscreen() const override;
    void drawGhost(Vector2 desplazamiento,Color relleno,Color borde) const override;
    Rectangle getBounds() const override;
    Color getColor() const override;

private:
    Rectangle rect;
    Vector2 vel;
};