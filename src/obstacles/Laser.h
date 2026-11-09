#pragma once
#include "obstacles/Obstacle.h"

class Laser:public Obstacle{
public:
    Laser(Vector2 pos,Vector2 dir,float vel);

    void update(float dt) override;
    void draw(float alfa) const override;
    bool collidesWith(Vector2 centro,float radio) const override;
    bool isOffscreen() const override;
    void drawGhost(Vector2 desplazamiento,Color relleno,Color borde) const override;
    Rectangle getBounds() const override;
    Color getColor() const override;

private:
    Vector2 pos; //punta delantera
    Vector2 dir; //direccion de vuelo (largo 1)
    float vel;
    float edad;
    Vector2 cola() const; //extremo de atras
};