#pragma once
#include <algorithm>
#include <cmath>
#include "raylib.h"
using namespace std;

//distancia de un punto al segmento a-b
inline float distanciaASegmento(Vector2 p,Vector2 a,Vector2 b){
    float abx=b.x-a.x;
    float aby=b.y-a.y;
    float l2=abx*abx+aby*aby;
    float t=(l2>0) ? ((p.x-a.x)*abx+(p.y-a.y)*aby)/l2 : 0.0F;
    t=clamp(t,0.0F,1.0F);
    float dx=p.x-(a.x+abx*t);
    float dy=p.y-(a.y+aby*t);
    return sqrt(dx*dx+dy*dy);
}