#include <algorithm>
#include "challenges/FaintChallenge.h"
using namespace std;

namespace{
    const float ALFA_MIN=0.10F;
    const float DIST_LEJOS=380.0F;
    const float DIST_CERCA=120.0F;
}

void FaintChallenge::update(float,bool,Context&){}

float FaintChallenge::alfaObstaculo(float distancia) const {
    float t=clamp((DIST_LEJOS-distancia)/(DIST_LEJOS-DIST_CERCA),0.0F,1.0F);
    return ALFA_MIN+(1.0F-ALFA_MIN)*t;
}