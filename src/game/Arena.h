#pragma once
#include "raylib.h"

namespace Arena{
    constexpr float HUD_ALTO=60.0F;

    //Rectangulo donde se juega
    inline Rectangle area(){
        return {0.0F,HUD_ALTO,(float)GetScreenWidth(),(float)GetScreenHeight()-HUD_ALTO};
    }

    inline Vector2 centro(){
        Rectangle a=area();
        return {a.x+a.width/2,a.y+a.height/2};
    }

    //Punto aleatorio del area, a mas de "dist" de "evitar"
    inline Vector2 puntoLibre(Vector2 evitar,float dist,float margen){
        Rectangle a=area();
        Vector2 p;
        do{
            p={(float)GetRandomValue((int)(a.x+margen),(int)(a.x+a.width-margen)),(float)GetRandomValue((int)(a.y+margen),(int)(a.y+a.height-margen))};
        }while(CheckCollisionPointCircle(p,evitar,dist));
        return p;
    }
}