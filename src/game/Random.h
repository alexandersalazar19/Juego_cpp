#pragma once
#include "raylib.h"

//multiplica t por un factor aleatorio entre 0.7 y 1.3
inline float variar(float t){
    return t*(0.7F+GetRandomValue(0,60)/100.0F);
}