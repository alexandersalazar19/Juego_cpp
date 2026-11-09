#include <cmath>
#include <algorithm>
#include "raylib.h"
#include "ui/Hud.h"
#include "game/Arena.h"
#include "game/Difficulty.h"
#include "game/Theme.h"
using namespace std;

void dibujarRejilla(){
    Rectangle a=Arena::area();
    const int paso=80;
    const int m=40; //el temblor mueve el mundo unos pixeles
    for(int x=0;x<=(int)(a.x+a.width)+m;x+=paso) DrawLine(x,(int)a.y-m,x,(int)(a.y+a.height)+m,Theme::REJILLA);
    for(int y=(int)a.y;y<=(int)(a.y+a.height)+m;y+=paso) DrawLine((int)a.x-m,y,(int)(a.x+a.width)+m,y,Theme::REJILLA);
}

namespace{
    Color mezclar(Color a,Color b,float t){
        return Color{(unsigned char)(a.r+(b.r-a.r)*t),(unsigned char)(a.g+(b.g-a.g)*t),(unsigned char)(a.b+(b.b-a.b)*t),(unsigned char)(a.a+(b.a-a.a)*t)};
    }

    //rectangulo con dos esquinas cortadas (arriba izquierda y abajo derecha), en sentido antihorario
    void formaBoton(Rectangle r,float c,Vector2 p[6]){
        p[0]={r.x+c,r.y};
        p[1]={r.x,r.y+c};
        p[2]={r.x,r.y+r.height};
        p[3]={r.x+r.width-c,r.y+r.height};
        p[4]={r.x+r.width,r.y+r.height-c};
        p[5]={r.x+r.width,r.y};
    }

    void rellenar(const Vector2 p[6],Color col){
        Vector2 cen={0.0F,0.0F};
        for(int i=0;i<6;i++){
            cen.x+=p[i].x/6.0F;
            cen.y+=p[i].y/6.0F;
        }
        Vector2 abanico[8]={cen,p[0],p[1],p[2],p[3],p[4],p[5],p[0]};
        DrawTriangleFan(abanico,8,col);
    }

    void contorno(const Vector2 p[6],float grosor,Color col){
        for(int i=0;i<6;i++) DrawLineEx(p[i],p[(i+1)%6],grosor,col);
    }

    Rectangle rectBoton(int i){
        const float W=(float)GetScreenWidth();
        const float H=(float)GetScreenHeight();
        const float w=min(640.0F,W*0.6F);
        const float h=84.0F;
        const float sep=22.0F;
        float total=NUM_DIFICULTADES*h+(NUM_DIFICULTADES-1)*sep;
        float y0=H/2-total/2+50.0F;
        return {(W-w)/2,y0+i*(h+sep),w,h};
    }
}

int botonBajoMouse(){
    Vector2 m=GetMousePosition();
    for(int i=0;i<NUM_DIFICULTADES;i++){
        if(CheckCollisionPointRec(m,rectBoton(i))) return i;
    }
    return -1;
}

void dibujarMenu(int seleccion){
    const int W=GetScreenWidth();
    const int H=GetScreenHeight();
    static float anim[NUM_DIFICULTADES]={}; //0 = reposo, 1 = seleccionado (transicion suave)
    float dt=GetFrameTime();

    //titulo
    const char* titulo="Selecciona la dificultad";
    int ancho=MeasureText(titulo,48);
    int ty=(int)rectBoton(0).y-110;
    DrawText(titulo,(W-ancho)/2,ty,48,Theme::TEXTO);
    DrawRectangle((W-ancho)/2,ty+62,ancho,3,Theme::JUGADOR);

    for(int i=0;i<NUM_DIFICULTADES;i++){
        float meta=(i==seleccion) ? 1.0F : 0.0F;
        anim[i]+=(meta-anim[i])*min(1.0F,14.0F*dt);
        float k=anim[i];

        Rectangle r=rectBoton(i);
        r.x+=14.0F*k; //el seleccionado se adelanta un poco
        const float c=18.0F; //tamano de la esquina cortada
        Vector2 p[6];

        //resplandor alrededor del seleccionado
        for(int g=2;g>=1;g--){
            Rectangle gr={r.x-g*7.0F,r.y-g*7.0F,r.width+g*14.0F,r.height+g*14.0F};
            formaBoton(gr,c+g*3.0F,p);
            rellenar(p,Fade(Theme::JUGADOR,0.06F*k));
        }

        formaBoton(r,c,p);
        rellenar(p,mezclar(Theme::PANEL,Theme::HUD,k)); //se oscurece al seleccionarlo
        contorno(p,2.0F,mezclar(Theme::BORDE,Theme::JUGADOR,k));
        DrawRectangle((int)r.x,(int)(r.y+c),4,(int)(r.height-c),Fade(Theme::JUGADOR,k)); //barra de acento

        //numero en un recuadro
        const float tag=36.0F;
        Rectangle t={r.x+26.0F,r.y+(r.height-tag)/2,tag,tag};
        DrawRectangleLinesEx(t,2.0F,mezclar(Theme::BORDE,Theme::JUGADOR,k));
        const char* num=TextFormat("%d",i+1);
        DrawText(num,(int)(t.x+(tag-MeasureText(num,24))/2),(int)(t.y+(tag-24)/2),24,mezclar(Theme::TEXTO_SEC,Theme::JUGADOR,k));

        //nombre
        DrawText(getConfig(i).nombre,(int)(t.x+tag+22),(int)(r.y+(r.height-34)/2),34,mezclar(Theme::TEXTO_SEC,Theme::TEXTO,k));

        //segmentos de dificultad
        const float sw=16.0F,sh=8.0F,sg=5.0F;
        float sx=r.x+r.width-30.0F-NUM_DIFICULTADES*(sw+sg)+sg;
        for(int s=0;s<NUM_DIFICULTADES;s++){
            Rectangle seg={sx+s*(sw+sg),r.y+(r.height-sh)/2,sw,sh};
            if(s<=i) DrawRectangleRec(seg,mezclar(Theme::TEXTO_SEC,Theme::JUGADOR,k));
            else DrawRectangleLinesEx(seg,1.0F,Fade(Theme::TEXTO_SEC,0.5F));
        }

        //multiplicador de puntos
        const char* pts=TextFormat("x%.1F puntos",getConfig(i).multPuntos);
        DrawText(pts,(int)(sx-24-MeasureText(pts,20)),(int)(r.y+(r.height-20)/2),20,mezclar(Theme::TEXTO_SEC,Theme::MONEDA,k));
    }
}

void dibujarHud(int puntos,const char* dificultad,int nivel,float tiempo){
    const int W=GetScreenWidth();
    const int alto=(int)Arena::HUD_ALTO;
    const int tam=28;
    const int y=(alto-tam)/2;
    const int margen=20;

    DrawRectangle(0,0,W,alto,Theme::HUD);
    DrawRectangle(0,alto-2,W,2,Theme::JUGADOR); //linea de neon en el borde inferior

    //Puntaje
    DrawText(TextFormat("Puntaje: %d",puntos),margen,y,tam,Theme::TEXTO);

    //Tiempo
    int min=(int)tiempo/60;
    int seg=(int)tiempo%60;
    const char* textTiempo=TextFormat("%02d:%02d",min,seg);
    DrawText(textTiempo,W/4-MeasureText(textTiempo,tam)/2,y,tam,Theme::TEXTO);

    //Dificultad
    DrawText(dificultad,(W-MeasureText(dificultad,tam))/2,y,tam,Theme::TEXTO);

    //Nivel
    const char* textNivel=TextFormat("Nivel %d",nivel);
    DrawText(textNivel,W-MeasureText(textNivel,tam)-margen,y,tam,Theme::TEXTO);
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
        if(i<jugador.getCargas()) DrawCircleV(c,r,Theme::JUGADOR);
        else{
            DrawCircleLinesV(c,r,Theme::JUGADOR);
            if(i==jugador.getCargas()) DrawCircleSector(c,r,-90.0F,-90.0F+360.0F*jugador.getProgresoRecarga(),32,Fade(Theme::JUGADOR,0.5F));
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
        if(i<jugador.getVidas()) DrawCircleV(c,r,Theme::VIDA_HUD);
        else DrawCircleLinesV(c,r,Fade(Theme::VIDA_HUD,0.5F));
    }
}

void dibujarAnuncio(int nivel,const char* reto,float progreso){
    Vector2 m=Arena::centro();
    float alfa=min(1.0F,sin(PI*progreso)*2.0F); //aparece, se mantiene y se va
    const char* t1=TextFormat("NIVEL %d",nivel);
    DrawText(t1,(int)m.x-MeasureText(t1,80)/2,(int)m.y-70,80,Fade(Theme::TEXTO,alfa));
    DrawText(reto,(int)m.x-MeasureText(reto,36)/2,(int)m.y+20,36,Fade(Theme::JUGADOR,alfa));
}

void dibujarAlertaBomba(float urgencia){ //urgencia: 0.0 recien puesta, 1.0 a punto de explotar
    Rectangle a=Arena::area();
    float pulso=0.5F+0.5F*sin((float)GetTime()*(5.0F+10.0F*urgencia));
    float alfa=(0.18F+0.32F*urgencia)*(0.4F+0.6F*pulso);
    Color r=Fade(Theme::PELIGRO,alfa);
    Color t=Fade(Theme::PELIGRO,0.0F);
    const int g=90; //grosor del resplandor
    DrawRectangleGradientV((int)a.x,(int)a.y,(int)a.width,g,r,t); //arriba
    DrawRectangleGradientV((int)a.x,(int)(a.y+a.height)-g,(int)a.width,g,t,r); //abajo
    DrawRectangleGradientH((int)a.x,(int)a.y,g,(int)a.height,r,t); //izquierda
    DrawRectangleGradientH((int)(a.x+a.width)-g,(int)a.y,g,(int)a.height,t,r); //derecha
}