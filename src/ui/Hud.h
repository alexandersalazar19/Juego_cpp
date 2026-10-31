#pragma once
#include "entities/Player.h"

void dibujarMenu();
void dibujarHud(int puntos,const char* dificultad,int nivel,float tiempo);
void dibujarCargas(const Player& jugador);