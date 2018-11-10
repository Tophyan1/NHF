#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "megjelenites.h"
#include "jatek.h"


static const char GlobalFont[] = "C:\\Windows\\Fonts\\LATINWD.TTF";

void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer){
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}


void szoveg_kiir_kozep(SDL_Renderer *renderer, Szoveg szov, SDL_Rect *gombok){

    SDL_Surface *felirat = TTF_RenderUTF8_Solid(szov.font, szov.str, szov.szin);
    SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    szov.hova.x = (1280 - felirat->w) /2;
    szov.hova.w = felirat->w;
    szov.hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &(szov.hova));
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
    gombok->x = szov.hova.x;
    gombok->h = szov.hova.h;
    gombok->w = szov.hova.w;
}
void szoveg_kiir(SDL_Renderer *renderer, Szoveg szov, SDL_Rect *gombok){

    SDL_Surface *felirat = TTF_RenderUTF8_Solid(szov.font, szov.str, szov.szin);
    SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    szov.hova.w = felirat->w;
    szov.hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &(szov.hova));
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
    gombok->x = szov.hova.x;
    gombok->h = szov.hova.h;
    gombok->w = szov.hova.w;
}


void menu(SDL_Renderer *renderer, SDL_Rect *gombok){
    SDL_RenderClear(renderer);
    TTF_Init();
    Szoveg szov;
    szov.font = TTF_OpenFont(GlobalFont, 72);
    if (!szov.font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    szov.hova.x = 0;
    szov.hova.y = 50;
    szov.hova.w = 0;
    szov.hova.h = 0;
    szov.szin.r = 255;
    szov.szin.g = 255;
    szov.szin.b = 255;
    szov.szin.a = 255;
    strcpy(szov.str, "Charge!");
    szoveg_kiir_kozep(renderer, szov, &gombok[0]);
    TTF_CloseFont(szov.font);
    szov.font = TTF_OpenFont("C:\\Windows\\Fonts\\LATINWD.TTF", 30);
    if (!szov.font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    int y[4] = {200, 275, 350, 425};
    char szoveg[4][50] = {"New Game", "Continue Game", "View Hall of Fame", "Exit"};
    for (int i = 0; i < 4; i++){
        szov.hova.y = y[i];
        strcpy(szov.str, szoveg[i]);
        szoveg_kiir_kozep(renderer, szov, &gombok[i]);
        gombok[i].y = szov.hova.y;

    }
    TTF_CloseFont(szov.font);
    SDL_RenderPresent(renderer);
}

bool Button_Pressed(SDL_Event ev, SDL_Rect gomb, SDL_Point p){
    return  ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT && SDL_PointInRect(&p, &gomb);
}

void HallOfFame_kiir(Champion *Hall, const int HoF_Nr, SDL_Renderer *renderer, SDL_Rect *gomb){
    Szoveg szov;
    szov.font = TTF_OpenFont(GlobalFont, 60);
    szov.hova.y = 50;
    szov.szin.r = 255;
    szov.szin.g = 255;
    szov.szin.b = 255;
    szov.szin.a = 255;
    strcpy(szov.str, "Hall of Fame");
    SDL_RenderClear(renderer);
    szoveg_kiir_kozep(renderer, szov, gomb);
    TTF_CloseFont(szov.font);
    szov.font = TTF_OpenFont(GlobalFont, 20);

    for (int i = 0; i < HoF_Nr; i++){
        szov.hova.y = 130+i*40;
        sprintf(szov.str, "%s:    %d", Hall[i].Name, Hall[i].Score);
        szoveg_kiir_kozep(renderer, szov, gomb);


    }




    szov.font = TTF_OpenFont(GlobalFont, 40);
    szov.hova.y = (720-50-40);
    strcpy(szov.str, "Back");
    szoveg_kiir_kozep(renderer, szov, gomb);
    TTF_CloseFont(szov.font);
    gomb->y = szov.hova.y;

}

void HallOfFame(SDL_Renderer *renderer){
    FILE *hallOfFame;
    hallOfFame = fopen("HallOfFame.txt", "r");
    if (hallOfFame == NULL){
        perror("File could not be opened!\n");
        exit(1);
    }
    const int HoF_Nr = 11;
    Champion *Hall = malloc(HoF_Nr*sizeof(Champion));
    for (int i = 0; i < HoF_Nr; i++){
        char temp[100] = {0};
        if (fgets(temp, 99, hallOfFame) != NULL){
            sscanf(temp, "%d\t%s",&(Hall[i].Score), Hall[i].Name);
        }
        else{
            strcpy(Hall[i].Name, "No person yet");
            Hall[i].Score = 0;
        }
    }
    fclose(hallOfFame);
    SDL_Rect gomb;
    HallOfFame_kiir(Hall, HoF_Nr, renderer, &gomb);
    free(Hall);
    SDL_RenderPresent(renderer);
    SDL_Event ev;
    SDL_Point p;
    while (SDL_WaitEvent(&ev)){
        p.x = ev.button.x;
        p.y = ev.button.y;
        if (Button_Pressed(ev, gomb, p)){
            return;
        }
    }
}



