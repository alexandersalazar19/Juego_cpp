#pragma once
#include "entities/Player.h"
#include "game/Difficulty.h"

//lo que los retos y power-ups pueden tocar
struct Context{
    Player& jugador;
    const ConfigDificultad& cfg;
    int nivel;
    int& puntos;
};