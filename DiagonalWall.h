#pragma once
#include "Obstacle.h"

class DiagonalWall:public Obstacle{
public:
    DiagonalWall(Vector2 centro,Vector2 tam,float angulo,Vector2 vel);

    void update(float dt) override;
    void draw() const override;
    bool collidesWith(Vector2 centroJugador,float radio) const override;
    bool isOffscreen() const override;

private:
    Vector2 centro;
    Vector2 tam; //ancho y alto
    float angulo; //grados
    Vector2 vel;
    float radioEnvolvente; //circulo que contiene todo el muro
};