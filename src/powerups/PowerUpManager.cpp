#include "powerups/PowerUpManager.h"
#include "game/Arena.h"
#include "game/Random.h"

void PowerUpManager::reiniciar(const ConfigDificultad& cfg){
    pickups.clear();
    desafio=0.0F;
    proxVida=variar(cfg.vidaCada);
}

void PowerUpManager::alCambiarNivel(){
    pickups.clear();
}

void PowerUpManager::alRecibirGolpe(){
    desafio=0.0F;
}

void PowerUpManager::update(float dt,bool activo,Context& ctx){
    //aparicion
    if(activo && ctx.cfg.vidaCada>0){
        proxVida-=dt;
        if(proxVida<=0){
            bool hayVida=false;
            for(const auto& p:pickups) if(p.getTipo()==P_VIDA) hayVida=true;
            if(!hayVida && desafio<=0 && ctx.jugador.getVidas()<ctx.jugador.getMaxVidas())
                pickups.push_back(Pickup(P_VIDA,Arena::puntoLibre(ctx.jugador.getPos(),200.0F,40.0F)));
            proxVida=variar(ctx.cfg.vidaCada);
        }
    }

    //recoger o caducar
    for(auto it=pickups.begin();it!=pickups.end();){
        it->update(dt);
        if(it->collidesWith(ctx.jugador.getPos(),ctx.jugador.getRadio())){
            switch(it->getTipo()){
                case P_VIDA: desafio=TIEMPO_DESAFIO; break;
            }
            it=pickups.erase(it);
        }else if(it->haExpirado()) it=pickups.erase(it);
        else it++;
    }

    //vida
    if(desafio>0){
        desafio-=dt;
        if(desafio<=0){
            desafio=0.0F;
            if(!ctx.jugador.ganarVida()) ctx.puntos+=puntosPor(200,ctx.cfg,ctx.nivel); //ya tenia el maximo
        }
    }
}

void PowerUpManager::dibujar(const Player& jugador) const {
    for(const auto& p:pickups) p.draw();
    if(desafio>0) DrawRing(jugador.getPos(),jugador.getRadio()+5,jugador.getRadio()+8,-90.0F,-90.0F+360.0F*(desafio/TIEMPO_DESAFIO),32,GREEN);
}