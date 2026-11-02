#pragma once
#include <vector>
#include "powerups/Pickup.h"
#include "game/Context.h"

class PowerUpManager{
public:
    void reiniciar(const ConfigDificultad& cfg);
    void alCambiarNivel();
    void alRecibirGolpe(); //cancela la vida
    void update(float dt,bool activo,Context& ctx);
    void dibujar(const Player& jugador) const;

private:
    std::vector<Pickup> pickups;
    float proxVida=0.0F;
    float desafio=0.0F; //segundos que quedan para obtener vida (0=inactivo)
};