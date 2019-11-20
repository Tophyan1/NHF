#ifndef MEGJELENITES_H
#define MEGJELENITES_H
#include "jatek.h"
#include "lista.h"


/** A programban használt betûtípus*/
#define  GLOBALFONT "C:\\Windows\\Fonts\\LATINWD.TTF"

/** Tetszõleges szöveg adatait tárolja(megjelenítés helye, szöveg színe, betûtípusa, a szöveg maga)  */
typedef struct Szoveg{
    SDL_Rect hova;              //A szöveg helye
    SDL_Color szin;             //A szöveg színe
    TTF_Font *font;             //A betűtípus
    char str[200];              //A szöveg
}Szoveg;

/** A dicsõséglista eleme    */
typedef struct Champion{
    int Score;                   //A győztes pontja
    char Name[51];               //A győztes neve
}Champion;

void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);
void szoveg_kiir_kozep(SDL_Renderer *renderer, Szoveg szov, SDL_Rect *gombok);
void szoveg_kiir(SDL_Renderer *renderer, Szoveg szov, SDL_Rect *gombok);
void menu(SDL_Renderer *renderer, SDL_Rect *gombok);
bool Button_Pressed(SDL_Event ev, SDL_Rect gomb, SDL_Point p);
void HallOfFame_kiir(Champion *Hall,const int HoF_Nr, SDL_Renderer *renderer, SDL_Rect *gomb);
void Valaszt(char *szo, char kar, char *szo1, char *szo2);
void HallOfFame(SDL_Renderer *renderer);
bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer);
void GetName(Gamestate *state, SDL_Renderer *renderer);
void GameCntrllr(SDL_Renderer *renderer, SDL_Rect *gombok, Gamestate state, int charge);
void GameLevelDraw(SDL_Renderer *renderer, Gamestate *state);
void InHoF(Player player);
void Won(SDL_Renderer *renderer, Gamestate state);

#endif //MEGJELENITES_H
