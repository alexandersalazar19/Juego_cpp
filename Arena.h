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
}