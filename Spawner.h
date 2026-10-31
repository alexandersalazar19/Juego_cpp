#pragma once
#include <memory>
#include <vector>
#include "Obstacle.h"
#include "Difficulty.h"
using namespace std;
using ListaObstaculos=vector<unique_ptr<Obstacle>>;

void spawnObstaculos(ListaObstaculos& out,float velocidad,const ConfigDificultad& cfg);