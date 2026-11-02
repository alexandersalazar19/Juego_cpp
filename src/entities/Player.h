#pragma once
#include "raylib.h"

class Player{
public:
    Player(float x,float t);

    void update(float dt);
    void draw() const;
    
    bool esInvulnerable() const;
    void recargarCargas();
    
    bool recibirGolpe();
    bool estaMuerto() const;
    bool ganarVida();
    
    Vector2 getPos() const;
    float getRadio() const;
    int getCargas() const;
    int getMaxCargas() const;
    float getProgresoRecarga() const;
    int getVidas() const;
    int getMaxVidas() const;

private:
    Vector2 pos;
    float radio;
    float velocidad;

    int cargas;
    float recarga;
    float invul;
    int vidas;
    Vector2 destinoTp() const;
};