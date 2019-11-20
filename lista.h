#ifndef LISTA_H
#define LISTA_H
#include "jatek.h"


/** Tetszõleges vektor   */
typedef struct Vector{
    double vx;          //x irányú komponens
    double vy;          //y irányú komponens
}Vector;

/** Pályán található részecske   */
typedef struct Particle{
    SDL_Point pos;      //A részecske helye
    int radius;         //A részecske sugara
    int charge;         //A részecske töltése
    int mass;           //A részecske tömege
    Vector vel;         //A részecske sebessége
    Vector acc;         //A részecske gyorsulása
}Particle;

/** Listaelem, amely a pályára lerakott részecskéket tartalmazza */
typedef struct ParticleList{
    Particle part;                  //A lista eleme; részecske
    struct ParticleList *next;      //A következő elemre mutató pointer
}ParticleList;

/** Listaelem, amely a pályán levõ falat tartalmazza */
typedef struct WallList{
    SDL_Rect wall;                  //A lista eleme; egy fal
    struct WallList *next;          //A következő elemre mutató pointer
}WallList;


ParticleList *addPart(ParticleList *first, Particle data);
WallList *addWall(WallList *first, SDL_Rect data);
Particle newPart(int x, int y, int charge);
void freePart(ParticleList **first);
void freeWall(WallList **first);

#endif // LISTA_H
