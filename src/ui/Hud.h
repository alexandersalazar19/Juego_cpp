#pragma once
#include "entities/Player.h"

void dibujarRejilla();
void dibujarMenu(int seleccion);
int botonBajoMouse(); //indice del boton bajo el cursor, o -1
void dibujarHud(int puntos,const char* dificultad,int nivel,float tiempo);
void dibujarCargas(const Player& jugador);
void dibujarVidas(const Player& jugador);
void dibujarAnuncio(int nivel,const char* reto,float progreso);
void dibujarAlertaBomba(float urgencia);