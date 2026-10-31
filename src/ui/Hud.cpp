#include "raylib.h"
#include "ui/Hud.h"
#include "game/Arena.h"
#include "game/Difficulty.h"

void dibujarMenu(){
    const int W=GetScreenWidth();
    const int H=GetScreenHeight();

    const char* titulo="Choose the difficulty";
    DrawText(titulo,(W-MeasureText(titulo,48))/2,H/4,48,BLACK);

    for(int i=0;i<NUM_DIFICULTADES;i++){
        const char* linea=TextFormat("%d - %s  (x%.1F puntos)",i+1,getConfig(i).nombre,getConfig(i).multPuntos);
        DrawText(linea,(W-MeasureText(linea,36))/2,H/3+40+i*60,36,DARKGRAY);
    }
}

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