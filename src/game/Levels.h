#pragma once
#include <string>

//cada reto es un bit: se combinan con | y se consultan con &
enum Modificador{
    MOD_NINGUNO=0,
    MOD_BOMBA=1<<0,
    MOD_TENUE=1<<1,
    MOD_OSCURO=1<<2,
    MOD_CALOR=1<<3
};

enum class FaseNivel{Anuncio,Activo,Cierre};

class ControlNiveles{
public:
    ControlNiveles();
    void reiniciar(int dificultad);
    bool update(float dt,bool hayObstaculos); //true el frame en que se supera un nivel
    int getNivel() const;
    int getMods() const;
    FaseNivel getFase() const;
    bool puedeSpawnear() const;
    float getRampa() const;
    float getProgresoAnuncio() const; //0.0 a 1.0
private:
    int nivel;
    int dificultad;
    int mods;
    FaseNivel fase;
    float tiempoFase;
    void siguienteNivel();
};

std::string nombreMods(int mods);