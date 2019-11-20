#ifndef JATEK_H
#define JATEK_H
#include "lista.h"
#include "megjelenites.h"



/** A p�lya �sszes adat�t tartalmaz� strukt�ra   */
typedef struct Level{
    ParticleList *parts;        //R�szecsk�k list�ja
    Particle part;              //A j�t�kban mozg� r�szecske
    int lvlNr;                  //A szint sz�ma
    int partNr;                 //A szinten lerakott t�lt�sek sz�ma
    SDL_Point orPos;            //A r�szecske eredeti helye
    Vector orVel;               //A r�szecske eredeti sebess�ge
    WallList *Walls;            //A szinten l�v� falak list�ja
    SDL_Rect goal;              //A c�lmez�
}Level;

/** A j�t�kos �sszes adat�t (n�v �s pont) tartalmaz� strukt�ra   */
typedef struct Player{         //A jobb olvashat�s�g�rt k�l�nb a Chamipon-t�l
    char Name[51];              //A j�t�kos neve
    int Score;                  //A j�t�kos megszerzett pontja
}Player;

/** A j�t�k �ppeni �ll�s�t tartalmaz� strukt�ra  */
typedef struct Gamestate{
    Player player;              //A j�t�kos
    Level level;                //Az aktu�lis szint
    int TryNr;                  //Az eddigi pr�b�lkoz�sok sz�ma

}Gamestate;

int HoFCount();
bool TestCollision(Particle part, SDL_Rect Wall);
Vector Force(Particle p1, Particle p2);
SDL_Point NextPos(SDL_Point p, Vector v, int t);
Level GetLevel(FILE* fp);
void SaveGame(Gamestate state);
bool LoadGame(Gamestate *state);
Vector SumForce(Particle part, ParticleList *first);
void GamePlay(SDL_Renderer *renderer, Gamestate *state, SDL_Rect *gombok, SDL_Rect field, int charge, bool *win);
void GameEventLoop(SDL_Renderer *renderer, Gamestate *state, SDL_Rect *gombok, int *charge, bool *win);
void Game(SDL_Renderer *renderer, Gamestate *state);


#endif // JATEK_H
