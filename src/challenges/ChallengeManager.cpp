#include "challenges/ChallengeManager.h"
#include "game/Levels.h"
#include "challenges/BombChallenge.h"
#include "challenges/FaintChallenge.h"
#include "challenges/DarkChallenge.h"
using namespace std;

void ChallengeManager::configurar(int mods,const ConfigDificultad& cfg){
    lista.clear();
    if(mods&MOD_BOMBA) lista.push_back(make_unique<BombChallenge>(cfg));
    if(mods&MOD_TENUE) lista.push_back(make_unique<FaintChallenge>());
    if(mods&MOD_OSCURO) lista.push_back(make_unique<DarkChallenge>());
    //aqui ira MOD_CALOR
}

void ChallengeManager::limpiar(){
    lista.clear();
}

void ChallengeManager::update(float dt,bool activo,Context& ctx){
    for(auto& r:lista) r->update(dt,activo,ctx);
}

void ChallengeManager::dibujarCapa(const Context& ctx) const {
    for(const auto& r:lista) r->dibujarCapa(ctx);
}

void ChallengeManager::dibujarMundo() const {
    for(const auto& r:lista) r->dibujarMundo();
}

void ChallengeManager::dibujarAlerta(const Context& ctx) const {
    for(const auto& r:lista) r->dibujarAlerta(ctx);
}

float ChallengeManager::alfaObstaculo(float distancia) const {
    float alfa=1.0F;
    for(const auto& r:lista) alfa*=r->alfaObstaculo(distancia);
    return alfa;
}