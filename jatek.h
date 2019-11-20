#ifndef JATEK_H
#define JATEK_H
#include "lista.h"
#include "megjelenites.h"



/** A pálya összes adatát tartalmazó struktúra   */
typedef struct Level{
    ParticleList *parts;        //Részecskék listája
    Particle part;              //A játékban mozgó részecske
    int lvlNr;                  //A szint száma
    int partNr;                 //A szinten lerakott töltések száma
    SDL_Point orPos;            //A részecske eredeti helye
    Vector orVel;               //A részecske eredeti sebessége
    WallList *Walls;            //A szinten lévõ falak listája
    SDL_Rect goal;              //A célmezõ
}Level;

/** A játékos összes adatát (név és pont) tartalmazó struktúra   */
typedef struct Player{         //A jobb olvashatóságért különb a Chamipon-tõl
    char Name[51];              //A játékos neve
    int Score;                  //A játékos megszerzett pontja
}Player;

/** A játék éppeni állását tartalmazó struktúra  */
typedef struct Gamestate{
    Player player;              //A játékos
    Level level;                //Az aktuális szint
    int TryNr;                  //Az eddigi próbálkozások száma

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
