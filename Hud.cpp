#include "raylib.h"
#include "Hud.h"
#include "Arena.h"

void dibujarHud(int puntos,const char* dificultad,int nivel,float tiempo){
    const int W=GetScreenWidth();
    const int alto=(int)Arena::HUD_ALTO;
    const int tam=28;
    const int y=(alto-tam)/2;
    const int margen=20;

    DrawRectangle(0,0,W,alto,BLACK);

    //Puntaje
    DrawText(TextFormat("Puntaje: %d",puntos),margen,y,tam,WHITE);

    //Tiempo
    int min=(int)tiempo/60;
    int seg=(int)tiempo%60;
    const char* textTiempo=TextFormat("%02d:%02d",min,seg);
    DrawText(textTiempo,W/4-MeasureText(textTiempo,tam)/2,y,tam,WHITE);

    //Dificultad
    DrawText(dificultad,(W-MeasureText(dificultad,tam))/2,y,tam,WHITE);

    //Nivel
    const char* textNivel=TextFormat("Nivel %d",nivel);
    DrawText(textNivel,W-MeasureText(textNivel,tam)-margen,y,tam,WHITE);
}