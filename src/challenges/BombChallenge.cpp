#include <algorithm>
#include <cmath>
#include "challenges/BombChallenge.h"
#include "game/Arena.h"
#include "game/Random.h"
#include "game/Theme.h"
#include "ui/Hud.h"
using namespace std;

namespace{
    const float DURACION=6.0F; //segundos hasta que explota
    const float RADIO=18.0F;

    void dibujarBomba(Vector2 pos,float restante){
        float f=restante/DURACION;
        bool alerta=(f<0.4F && (int)(restante*10)%2==0);
        DrawCircleV(pos,RADIO,alerta ? Theme::PELIGRO : Theme::CUERPO_BOMBA);
        DrawRing(pos,RADIO-2,RADIO,0.0F,360.0F,32,Theme::PELIGRO); //borde rojo
        DrawCircleV({pos.x-RADIO*0.35F,pos.y-RADIO*0.35F},RADIO*0.2F,Fade(Theme::TEXTO,0.35F)); //brillo

        Vector2 base={pos.x+RADIO*0.5F,pos.y-RADIO*0.85F}; //mecha
        Vector2 punta={pos.x+RADIO*1.0F,pos.y-RADIO*1.5F};
        DrawLineEx(base,punta,3.0F,Theme::TEXTO_SEC);
        float s=0.6F+0.4F*sin(restante*30.0F); //chispa que titila
        DrawCircleV(punta,3.0F+3.0F*s,Theme::AVISO);
        DrawCircleV(punta,2.0F,Theme::MONEDA);

        DrawRing(pos,RADIO+4,RADIO+7,-90.0F,-90.0F+360.0F*f,32,Theme::PELIGRO);
        const char* t=TextFormat("%d",(int)ceil(restante));
        DrawText(t,(int)pos.x-MeasureText(t,20)/2,(int)pos.y-10,20,Theme::TEXTO);
    }
}

BombChallenge::BombChallenge(const ConfigDificultad& cfg):cada(cfg.bombaCada),proxima(variar(cfg.bombaCada)){}

void BombChallenge::update(float dt,bool activo,Context& ctx){
    if(activo && cada>0){
        proxima-=dt;
        if(proxima<=0){
            bombas.push_back({Arena::puntoLibre(ctx.jugador.getPos(),200.0F,40.0F),DURACION});
            proxima=variar(cada);
        }
    }

    //la onda expansiva dana al jugador cuando su borde lo alcanza
    for(auto it=explosiones.begin();it!=explosiones.end();){
        it->t+=dt;
        float r=VELOCIDAD_ONDA_BOMBA*it->t;
        if(!it->golpeo){
            float dx=ctx.jugador.getPos().x-it->pos.x;
            float dy=ctx.jugador.getPos().y-it->pos.y;
            float d=sqrt(dx*dx+dy*dy);
            if(r>=d-ctx.jugador.getRadio()){ //el frente llego al jugador
                it->golpeo=true;
                bool yaPaso=(r>d+ctx.jugador.getRadio()+VELOCIDAD_ONDA_BOMBA*dt*2.0F); //te teletransportaste detras del frente
                if(!yaPaso) ctx.jugador.recibirGolpe();
            }
        }
        if(r>=Effects::alcanceOnda(it->pos)) it=explosiones.erase(it);
        else it++;
    }

    for(auto it=bombas.begin();it!=bombas.end();){
        it->restante-=dt;
        if(CheckCollisionCircles(it->pos,RADIO,ctx.jugador.getPos(),ctx.jugador.getRadio())){
            ctx.puntos+=puntosPor(150,ctx.cfg,ctx.nivel); //la desactivaste
            it=bombas.erase(it);
        }else if(it->restante<=0){
            ctx.efectos.explosion(it->pos);
            explosiones.push_back({it->pos,0.0F,false});
            it=bombas.erase(it);
        }else it++;
    }
}

void BombChallenge::dibujarMundo(const Context&) const {
    for(const auto& b:bombas) dibujarBomba(b.pos,b.restante);
}

void BombChallenge::dibujarAlerta(const Context&) const {
    if(bombas.empty()) return;
    float urgencia=0.0F; //la de la bomba mas cercana a explotar
    for(const auto& b:bombas) urgencia=max(urgencia,1.0F-b.restante/DURACION);
    dibujarAlertaBomba(urgencia);
}