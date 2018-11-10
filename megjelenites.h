#ifndef MEGJELENITES_H
#define MEGJELENITES_H

typedef struct Szoveg{
    SDL_Rect hova;
    SDL_Color szin;
    TTF_Font *font;
    char str[200];
}Szoveg;

typedef struct Champion{
    int Score;
    char Name[50];
}Champion;


void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);
void szoveg_kiir_kozep(SDL_Renderer *renderer, Szoveg szov, SDL_Rect *gombok);
void szoveg_kiir(SDL_Renderer *renderer, Szoveg szov, SDL_Rect *gombok);
void menu(SDL_Renderer *renderer, SDL_Rect *gombok);
bool Button_Pressed(SDL_Event ev, SDL_Rect gomb, SDL_Point p);
void HallOfFame_kiir(Champion *Hall,const int HoF_Nr, SDL_Renderer *renderer, SDL_Rect *gomb);
void HallOfFame(SDL_Renderer *renderer);



#endif //MEGJELENITES_H
