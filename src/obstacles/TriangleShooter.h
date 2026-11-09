#pragma once
#include <vector>
#include "obstacles/Obstacle.h"

class TriangleShooter:public Obstacle{
public:
    TriangleShooter(Vector2 centro,float radio,Vector2 vel,float giro,float cada,float velLaser);

    void update(float dt) override;
    void draw(float alfa) const override;
    bool collidesWith(Vector2 centro,float radio) const override;
    bool isOffscreen() const override;
    void drawGhost(Vector2 desplazamiento,Color relleno,Color borde) const override;
    Rectangle getBounds() const override;
    Color getColor() const override;
    void emitir(vector<unique_ptr<Obstacle>>& out) override;

private:
    struct Disparo{
        Vector2 pos;
        Vector2 dir;
    };

    Vector2 centro;
    float radio;
    Vector2 vel;
    float angulo; //grados
    float giro; //grados por segundo
    float cada; //segundos entre disparos
    float velLaser;
    float reloj; //tiempo desde el ultimo disparo
    float fase; //fase del parpadeo: cada vuelta completa es un parpadeo
    vector<Disparo> pendientes;

    void vertices(Vector2 v[3],Vector2 desp) const;
    bool dentroDelArea() const;
    bool avisando() const;
};