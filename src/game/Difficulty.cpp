#include <algorithm>
#include "Difficulty.h"
using namespace std;

namespace{
    const ConfigDificultad TABLA[NUM_DIFICULTADES]={
        {"Easy",200.0F,1.30F,1.0F,{4,1,0,2}},
        {"Normal",260.0F,1.10F,1.5F,{3,2,1,3}},
        {"Hard",320.0F,0.90F,2.0F,{3,3,3,3}},
        {"Extreme",380.0F,0.70F,3.0F,{2,3,4,4}},
    };
}

const ConfigDificultad& getConfig(int indice){
    return TABLA[clamp(indice,0,NUM_DIFICULTADES-1)];
}

int nivelPorTiempo(float tiempo){
    return 1+(int)(tiempo/SEGUNDOS_POR_NIVEL);
}

float velocidadActual(const ConfigDificultad& c,int nivel){
    float factor=min(1.0F+0.08F*(nivel-1),3.0F);
    return c.velocidadBase*factor;
}

float intervaloActual(const ConfigDificultad& c,int nivel){
    float intervalo=c.intervaloBase/(1.0F+0.05F*(nivel-1));
    return max(intervalo,0.35F);
}

int puntosPor(int puntosBase,const ConfigDificultad& c,int nivel){
    return (int)(puntosBase*c.multPuntos*nivel);
}