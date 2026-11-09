#pragma once
#include "raylib.h"

namespace Theme{
    //escenario
    constexpr Color FONDO={30,34,44,255};
    constexpr Color REJILLA={42,48,64,255};
    constexpr Color HUD={16,19,27,255};
    constexpr Color TEXTO={225,232,245,255};
    constexpr Color TEXTO_SEC={138,148,171,255};
    constexpr Color VELO={5,6,12,255}; //oscuridad del reto y de la muerte
    constexpr Color PANEL={40,47,62,255}; //boton en reposo
    constexpr Color BORDE={72,82,106,255}; //contorno en reposo

    //jugador y obstaculos
    constexpr Color JUGADOR={0,229,255,255};
    constexpr Color MURO={180,75,255,255};
    constexpr Color CIRCULO={255,60,180,255};

    //peligros, premios y avisos
    constexpr Color PELIGRO={255,59,48,255};
    constexpr Color CUERPO_BOMBA={14,16,22,255};
    constexpr Color VIDA_HUD={255,77,90,255};
    constexpr Color MONEDA={255,225,77,255};
    constexpr Color POWERUP={57,255,130,255};
    constexpr Color AVISO={255,159,28,255};

    //calor
    constexpr Color CALOR_FRIO={255,225,77,255};
    constexpr Color CALOR_MEDIO={255,159,28,255};
    constexpr Color CALOR_MAX={255,59,48,255};
}