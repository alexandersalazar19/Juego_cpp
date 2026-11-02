#include "challenges/ChallengeManager.h"
#include "challenges/BombChallenge.h"
#include "game/Levels.h"
using namespace std;

void ChallengeManager::configurar(int mods,const ConfigDificultad& cfg){
    lista.clear();
    if(mods&MOD_BOMBA) lista.push_back(make_unique<BombChallenge>(cfg));
    //aqui iran MOD_TENUE, MOD_OSCURO y MOD_CALOR
}

void ChallengeManager::update(float dt,bool activo,Context& ctx){
    for(auto& r:lista) r->update(dt,activo,ctx);
}

void ChallengeManager::dibujarMundo() const {
    for(const auto& r:lista) r->dibujarMundo();
}

void ChallengeManager::dibujarCapa(const Context& ctx) const {
    for(const auto& r:lista) r->dibujarCapa(ctx);
}