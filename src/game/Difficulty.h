#pragma once

enum TipoObstaculo{
    T_PARED_SIMPLE,
    T_PARED_DOBLE,
    T_PARED_DIAGONAL,
    T_CIRCULO,
    T_TRIANGULO,
    NUM_TIPOS
};

struct ConfigDificultad{
    const char* nombre;
    float velocidadBase;
    float intervaloBase;
    float multPuntos;
    float bombaCada; //segundos entre bombas (0 = nunca)
    float vidaCada; //segundos entre power-ups de vida (0 = nunca)
    float avisoBase; //segundos de aviso antes de que salga un obstaculo
    float probCombo; //probabilidad de que un nivel aleatorio combine 2 retos
    int pesos[NUM_TIPOS];
};

constexpr int NUM_DIFICULTADES=4;

float velocidadActual(const ConfigDificultad& c,int nivel,float rampa);
float intervaloActual(const ConfigDificultad& c,int nivel,float rampa);
int puntosPor(int puntosBase,const ConfigDificultad& c,int nivel);
float avisoActual(const ConfigDificultad& c,int nivel);

const ConfigDificultad& getConfig(int indice);