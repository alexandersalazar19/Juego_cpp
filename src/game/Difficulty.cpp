#include <algorithm>
#include "Difficulty.h"
using namespace std;

namespace{
    const ConfigDificultad TABLA[NUM_DIFICULTADES]={
        //nombre    vel     interv mult  bomba  vida   aviso combo  frec de obstaculos
        {"Fácil",   200.0F, 1.30F, 1.0F, 14.0F, 40.0F, 1.2F, 0.25F, {4,1,0,2,0}},
        {"Normal",  260.0F, 1.10F, 1.5F, 11.0F, 45.0F, 0.9F, 0.50F, {3,2,1,3,1}},
        {"Difícil", 320.0F, 0.90F, 2.0F, 9.0F,  50.0F, 0.7F, 0.75F, {3,3,3,3,2}},
        {"Extremo", 380.0F, 0.70F, 3.0F, 7.0F,  60.0F, 0.6F, 1.00F, {2,3,4,4,3}}
    };
}

const ConfigDificultad& getConfig(int indice){
    return TABLA[clamp(indice,0,NUM_DIFICULTADES-1)];
}

float velocidadActual(const ConfigDificultad& c,int nivel,float rampa){
    float factor=min(1.0F+0.06F*nivel,2.5F);
    return c.velocidadBase*factor*rampa;
}

float intervaloActual(const ConfigDificultad& c,int nivel,float rampa){
    float factor=min(1.0F+0.04F*nivel,2.0F);
    return max(c.intervaloBase/(factor*rampa),0.35F);
}

int puntosPor(int puntosBase,const ConfigDificultad& c,int nivel){
    return (int)(puntosBase*c.multPuntos*(nivel+1));
}

float avisoActual(const ConfigDificultad& c,int nivel){
    return c.avisoBase*max(1.0F-0.03F*nivel,0.75F);
}