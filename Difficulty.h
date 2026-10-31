#pragma once

enum TipoObstaculo{
    T_PARED_SIMPLE,
    T_PARED_DOBLE,
    T_PARED_DIAGONAL,
    T_CIRCULO,
    NUM_TIPOS
};

struct ConfigDificultad{
    const char* nombre;
    float velocidadBase;
    float intervaloBase;
    float multPuntos;
    int pesos[NUM_TIPOS];
};

constexpr int NUM_DIFICULTADES=4;
constexpr float SEGUNDOS_POR_NIVEL=20.0F;

const ConfigDificultad& getConfig(int indice);

int nivelPorTiempo(float tiempo);
float velocidadActual(const ConfigDificultad& c,int nivel);
float intervaloActual(const ConfigDificultad& c,int nivel);
int puntosPor(int puntosBase,const ConfigDificultad& c,int nivel);