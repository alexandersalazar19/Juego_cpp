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

void dibujarCargas(const Player& jugador){
    const int W=GetScreenWidth();
    const int r=14;
    const int sep=44;
    int n=jugador.getMaxCargas();
    int x0=W*3/4-(n-1)*sep/2;
    float y=Arena::HUD_ALTO/2;
    for(int i=0;i<n;i++){
        Vector2 c={(float)(x0+i*sep),y};
        if(i<jugador.getCargas()) DrawCircleV(c,r,SKYBLUE);
        else{
            DrawCircleLinesV(c,r,SKYBLUE);
            if(i==jugador.getCargas()) DrawCircleSector(c,r,-90.0F,-90.0F+360.0F*jugador.getProgresoRecarga(),32,Fade(SKYBLUE,0.5F));
        }
    }
}

void dibujarVidas(const Player& jugador){
    const int W=GetScreenWidth();
    const int r=12;
    const int sep=34;
    int n=jugador.getMaxVidas();
    int x0=W*3/8-(n-1)*sep/2;
    float y=Arena::HUD_ALTO/2;
    for(int i=0;i<n;i++){
        Vector2 c={(float)(x0+i*sep),y};
        if(i<jugador.getVidas()) DrawCircleV(c,r,RED);
        else DrawCircleLinesV(c,r,Fade(RED,0.5F));
    }
}