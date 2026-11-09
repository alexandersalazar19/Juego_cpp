#pragma once
#include "obstacles/Obstacle.h"

class CircleObstacle:public Obstacle{
public:
    CircleObstacle(Vector2 pos,float radio,Vector2 vel,float amplitud,float frecuencia);

    void update(float dt) override;
    void draw(float alfa) const override;
    bool collidesWith(Vector2 centro,float radio) const override;
    bool isOffscreen() const override;
    void drawGhost(Vector2 desplazamiento,Color relleno,Color borde) const override;
    Rectangle getBounds() const override;
    Color getColor() const override;

private:
    Vector2 base; //linea por la que avanza
    Vector2 pos; //posicion real (base + ondulacion)
    float radio;
    Vector2 vel;
    Vector2 perp; //direccion perpendicular a velocidad
    float amplitud; //desviacion hacia los lados
    float frecuencia; //cuanto ondula (rad/s)
    float tiempo;
};