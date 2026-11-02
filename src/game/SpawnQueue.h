#pragma once
#include <memory>
#include <vector>
#include "game/Spawner.h"
using namespace std;

class SpawnQueue{
public:
    void add(ListaObstaculos& nuevos,float retraso); //mueve los obstaculos de "nuevos" a la cola
    void update(float dt,ListaObstaculos& activos); //los que cumplieron su aviso pasan a "activos"
    void draw() const;
    bool empty() const;
    void clear();

private:
    struct Pendiente{
        unique_ptr<Obstacle> obs;
        float restante;
        float total;
    };
    vector<Pendiente> lista;
};