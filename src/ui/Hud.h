#pragma once
#include "entities/Player.h"

void dibujarMenu();
void dibujarHud(int puntos,const char* dificultad,int nivel,float tiempo);
void dibujarCargas(const Player& jugador);
void dibujarVidas(const Player& jugador);
void dibujarAnuncio(int nivel,const char* reto,float progreso);
void dibujarAlertaBomba(float urgencia);