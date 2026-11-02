#pragma once
#include <memory>
#include <vector>
#include "obstacles/Obstacle.h"
#include "Difficulty.h"
#include "entities/Pickup.h"
using namespace std;
using ListaObstaculos=vector<unique_ptr<Obstacle>>;

void spawnObstaculos(ListaObstaculos& out,float velocidad,const ConfigDificultad& cfg);
Pickup crearPickup(TipoPickup tipo,Vector2 evitar);