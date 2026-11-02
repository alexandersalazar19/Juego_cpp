#include <cmath>
#include "raylib.h"
#include "ui/Hud.h"
#include "game/Arena.h"
#include "game/Difficulty.h"
using namespace std;

void dibujarMenu(){
    const int W=GetScreenWidth();
    const int H=GetScreenHeight();

    const char* titulo="Selecciona la dificultad";
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

void dibujarAnuncio(int nivel,const char* reto,float progreso){
    Vector2 m=Arena::centro();
    float alfa=min(1.0F,sin(PI*progreso)*2.0F); //aparece, se mantiene y se va
    const char* t1=TextFormat("NIVEL %d",nivel);
    DrawText(t1,(int)m.x-MeasureText(t1,80)/2,(int)m.y-70,80,Fade(BLACK,alfa));
    DrawText(reto,(int)m.x-MeasureText(reto,36)/2,(int)m.y+20,36,Fade(DARKGRAY,alfa));
}

void dibujarAlertaBomba(float urgencia){ //urgencia: 0.0 recien puesta, 1.0 a punto de explotar
    Rectangle a=Arena::area();
    float pulso=0.5F+0.5F*sin((float)GetTime()*(5.0F+10.0F*urgencia));
    float alfa=(0.18F+0.32F*urgencia)*(0.4F+0.6F*pulso);
    Color r=Fade(RED,alfa);
    Color t=Fade(RED,0.0F);
    const int g=90; //grosor del resplandor
    DrawRectangleGradientV((int)a.x,(int)a.y,(int)a.width,g,r,t); //arriba
    DrawRectangleGradientV((int)a.x,(int)(a.y+a.height)-g,(int)a.width,g,t,r); //abajo
    DrawRectangleGradientH((int)a.x,(int)a.y,g,(int)a.height,r,t); //izquierda
    DrawRectangleGradientH((int)(a.x+a.width)-g,(int)a.y,g,(int)a.height,t,r); //derecha
}