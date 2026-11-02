#include "game/Levels.h"
#include "game/Difficulty.h"
#include "raylib.h"
using namespace std;

namespace{
    const float DURACION_ANUNCIO=2.5F;
    const float DURACION_NIVEL=30.0F;
    const float RAMPA_MIN=0.7F; //velocidad al empezar el nivel
    const float RAMPA_MAX=1.3F; //velocidad al terminarlo
    const int MAX_COMBO=2; //retos combinados en Extremo

    const int GUION[]={MOD_BOMBA,MOD_TENUE,MOD_OSCURO,MOD_CALOR};
    const char* NOMBRES[]={"Bombas","Poca Visibilidad","Oscuridad","Sobrecalentamiento"};
    const int N_MODS=sizeof(GUION)/sizeof(GUION[0]);

    const int PROHIBIDOS[]={MOD_TENUE|MOD_OSCURO};

    bool prohibido(int mods){
        for(int p:PROHIBIDOS) if((mods&p)==p) return true;
        return false;
    }

    int modsParaNivel(int nivel,int dificultad,int previo){
        if(nivel<=0) return MOD_NINGUNO;
        if(nivel<=N_MODS) return GUION[nivel-1];
        if(dificultad<NUM_DIFICULTADES-1){ //de facil a dificil: uno solo
            int m;
            do{
                m=GUION[GetRandomValue(0,N_MODS-1)];
            }while(m==previo && N_MODS>1);
            return m;
        }
        //Extremo: combinacion aleatoria
        int k=GetRandomValue(1,MAX_COMBO);
        int mods=MOD_NINGUNO;
        int intentos=0;
        while(k>0 && intentos<30){
            intentos++;
            int m=GUION[GetRandomValue(0,N_MODS-1)];
            if(mods&m) continue; //ya lo tenia
            if(prohibido(mods|m)) continue;
            mods|=m;
            k--;
        }
        return mods;
    }
}

ControlNiveles::ControlNiveles():nivel(0),dificultad(0),mods(MOD_NINGUNO),fase(FaseNivel::Anuncio),tiempoFase(0.0F){}

void ControlNiveles::reiniciar(int d){
    nivel=0;
    dificultad=d;
    mods=modsParaNivel(0,d,MOD_NINGUNO);
    fase=FaseNivel::Anuncio;
    tiempoFase=0.0F;
}

void ControlNiveles::siguienteNivel(){
    int previo=mods;
    nivel++;
    mods=modsParaNivel(nivel,dificultad,previo);
    fase=FaseNivel::Anuncio;
    tiempoFase=0.0F;
}

bool ControlNiveles::update(float dt,bool hayObstaculos){
    tiempoFase+=dt;
    switch(fase){
        case FaseNivel::Anuncio:
            if(tiempoFase>=DURACION_ANUNCIO){
                fase=FaseNivel::Activo;
                tiempoFase=0.0F;
            }
            break;
        case FaseNivel::Activo:
            if(tiempoFase>=DURACION_NIVEL){
                fase=FaseNivel::Cierre;
                tiempoFase=0.0F;
            }
            break;
        case FaseNivel::Cierre:
            if(!hayObstaculos){
                siguienteNivel();
                return true;
            }
            break;
    }
    return false;
}

bool ControlNiveles::puedeSpawnear() const{
    return fase==FaseNivel::Activo;
}

string nombreMods(int mods){
    if(mods==MOD_NINGUNO) return "Calentamiento";
    string s;
    for(int i=0;i<N_MODS;i++){
        if(mods&GUION[i]){
            if(!s.empty()) s+=" + ";
            s+=NOMBRES[i];
        }
    }
    return s;
}

int ControlNiveles::getNivel() const {return nivel;}
int ControlNiveles::getMods() const {return mods;}
FaseNivel ControlNiveles::getFase() const {return fase;}
float ControlNiveles::getProgresoAnuncio() const {return (fase==FaseNivel::Anuncio) ? tiempoFase/DURACION_ANUNCIO : 1.0F;}
float ControlNiveles::getRampa() const{
    if(fase==FaseNivel::Anuncio) return RAMPA_MIN;
    if(fase==FaseNivel::Cierre) return RAMPA_MAX;
    return RAMPA_MIN+(RAMPA_MAX-RAMPA_MIN)*(tiempoFase/DURACION_NIVEL);
}