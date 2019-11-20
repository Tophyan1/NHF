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
#include "lista.h"




/** InfoC-rõl származó grafikus ablakot inicializáló függvény*/
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

/**
 * Kiírja a megkapott szöveget középre igazítva, hozzárendel egy gombot
 * @param renderer a képernyõ, amire kiírja a szöveget
 * @param szov a szöveg adatai
 * @param gombok a gomb, amit hozzárendel a szöveghez
 */
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

/**
 * Kiírja a megkapott szöveget, hozzárendel egy gombot
 * @param renderer a képernyõ, amire kiírja a szöveget
 * @param szov a szöveg adatai
 * @param gombok a gomb, amit hozzárendel a szöveghez
 */
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
    gombok->y = szov.hova.y;
}

/**
 * A menü funkció megjelenítéséért felel
 * @param renderer a képernyõ
 * @param gombok azon gombok tömbje, amelyek hatással lesznek a program további futására
 */
void menu(SDL_Renderer *renderer, SDL_Rect *gombok){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    TTF_Init();
    Szoveg szov;
    szov.font = TTF_OpenFont(GLOBALFONT, 72);
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
    szov.font = TTF_OpenFont(GLOBALFONT, 30);
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

/**
 * Leteszteli, hogy megnyomtak-e egy megadott gombot.
 * @param ev az event, ami történt
 * @param gomb a gomb, amit meg kellett nyomni
 * @param p a kattintás helye
 * @return IGAZ, ha megnyomták a gombot, HAMIS, ha nem
 */
bool Button_Pressed(SDL_Event ev, SDL_Rect gomb, SDL_Point p){
    return  ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT && SDL_PointInRect(&p, &gomb);
}
/**
 * Megjeleníti a dicsõséglista elemeit.
 * @param Hall a lista elemei egy tömbben
 * @param HoF_Nr a lista darabszáma
 * @param renderer a képernyõ, ahová megjeleníti
 * @param gomb a visszalépés gombja
 */
void HallOfFame_kiir(Champion *Hall, const int HoF_Nr, SDL_Renderer *renderer, SDL_Rect *gomb){
    Szoveg szov;
    szov.font = TTF_OpenFont(GLOBALFONT, 60);
    szov.hova.y = 50;
    szov.szin.r = 255;
    szov.szin.g = 255;
    szov.szin.b = 255;
    szov.szin.a = 255;
    strcpy(szov.str, "Hall of Fame");
    SDL_RenderClear(renderer);
    szoveg_kiir_kozep(renderer, szov, gomb);
    TTF_CloseFont(szov.font);
    szov.font = TTF_OpenFont(GLOBALFONT, 20);
    for (int i = 0; i < HoF_Nr; i++){
        szov.hova.y = 130+i*40;
        sprintf(szov.str, "%s:    %d", Hall[i].Name, Hall[i].Score);
        szoveg_kiir_kozep(renderer, szov, gomb);
    }
    szov.font = TTF_OpenFont(GLOBALFONT, 40);
    szov.hova.y = (720-50-40);
    strcpy(szov.str, "Back");
    szoveg_kiir_kozep(renderer, szov, gomb);
    TTF_CloseFont(szov.font);
    gomb->y = szov.hova.y;

}

/**
 * 6. heti laborban megírt stringszétválasztó függvény.
 * @param szo az eredeti string
 * @param kar az elválasztó karakter
 * @param szo1 az elsõ string
 * @param szo2 a második string
 */
void Valaszt(char szo[], char kar, char szo1[], char szo2[]){
    int i = 0;
    int pos = 0;
    while (szo[i] != kar){
        szo1[pos++] = szo[i++];
    }
    szo1[pos] = '\0' ;
    i++;
    pos = 0;
    while (szo[i] != '\0'){
        szo2[pos++] = szo[i++];
    }
    szo2[pos-1] = '\0';
}

/** A dicsõséglista betöltéséért és a menüpont végrehajtásáért felelõs függvény*/
void HallOfFame(SDL_Renderer *renderer){
    FILE *hallOfFame;
    hallOfFame = fopen("HallOfFame.txt", "r");
    if (hallOfFame == NULL){
        perror("File could not be opened!\n");
        exit(1);
    }
    const int HoF_Nr = HoFCount();
    Champion *Hall = malloc(HoF_Nr*sizeof(Champion));
    for (int i = 0; i < HoF_Nr; i++){
        char temp[100] = {0};
        if (fgets(temp, 99, hallOfFame) != NULL){
            char tempNum[50];
            Valaszt(temp, '\t', tempNum, Hall[i].Name);
            sscanf(tempNum, "%d",&(Hall[i].Score));
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

/**
 * Beolvas egy szoveget a billentyuzetrol.
 * A rajzolashoz hasznalt font es a megjelenito az utolso parameterek.
 * Az elso a tomb, ahova a beolvasott szoveg kerul.
 * A masodik a maximális hossz, ami beolvasható.
 * A visszateresi erteke logikai igaz, ha sikerult a beolvasas. */
bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer) {
    /* Ez tartalmazza az aktualis szerkesztest */
    char composition[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    composition[0] = '\0';
    /* Ezt a kirajzolas kozben hasznaljuk */
    char textandcomposition[hossz + SDL_TEXTEDITINGEVENT_TEXT_SIZE + 1];
    /* Max hasznalhato szelesseg */
    int maxw = teglalap.w - 2;
    int maxh = teglalap.h - 2;

    dest[0] = '\0';

    bool enter = false;
    bool kilep = false;

    SDL_StartTextInput();
    while (!kilep && !enter) {
        /* doboz kirajzolasa */
        boxRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, hatter.r, hatter.g, hatter.b, 255);
        rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, szoveg.r, szoveg.g, szoveg.b, 255);
        /* szoveg kirajzolasa */
        int w;
        strcpy(textandcomposition, dest);
        strcat(textandcomposition, composition);
        if (textandcomposition[0] != '\0') {
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomposition, szoveg);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
            SDL_Rect cel = { teglalap.x, teglalap.y, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);
            w = cel.w;
        } else {
            w = 0;
        }
        /* kurzor kirajzolasa */
        if (w < maxw) {
            vlineRGBA(renderer, teglalap.x + w + 2, teglalap.y + 2, teglalap.y + teglalap.h - 3, szoveg.r, szoveg.g, szoveg.b, 192);
        }
        /* megjeleniti a képernyon az eddig rajzoltakat */
        SDL_RenderPresent(renderer);

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            /* Kulonleges karakter */
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    int textlen = strlen(dest);
                    do {
                        if (textlen == 0) {
                            break;
                        }
                        if ((dest[textlen-1] & 0x80) == 0x00)   {
                            /* Egy bajt */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0x80) {
                            /* Bajt, egy tobb-bajtos szekvenciabol */
                            dest[textlen-1] = 0x00;
                            textlen--;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0xC0) {
                            /* Egy tobb-bajtos szekvencia elso bajtja */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                    } while(true);
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    enter = true;
                }
                break;

            /* A feldolgozott szoveg bemenete */
            case SDL_TEXTINPUT:
                if (strlen(dest) + strlen(event.text.text) < hossz) {
                    strcat(dest, event.text.text);
                }

                /* Az eddigi szerkesztes torolheto */
                composition[0] = '\0';
                break;

            /* Szoveg szerkesztese */
            case SDL_TEXTEDITING:
                strcpy(composition, event.edit.text);
                break;

            case SDL_QUIT:
                /* visszatesszuk a sorba ezt az eventet, mert
                 * sok mindent nem tudunk vele kezdeni */
                SDL_PushEvent(&event);
                kilep = true;
                break;
        }
    }

    /* igaz jelzi a helyes beolvasast; = ha enter miatt allt meg a ciklus */
    SDL_StopTextInput();
    return enter;
}


/**
 * Inicializálja a változókat, hogy a játékos neve beolvasható
 * legyen. Ezután elkészíti az olvasáshoz a képernyőt, és
 * beoolvassa a játékos nevét.
 *
 *
 */
void GetName(Gamestate *state, SDL_Renderer *renderer){
    SDL_Rect textBox = {490, 300, 300, 40};
    SDL_Color textBckgrnd = { 0, 0, 255, 255 }, textText = { 0, 0, 0, 255 }, feher = { 255, 255, 255, 255 };
    TTF_Font *textFont = TTF_OpenFont(GLOBALFONT, 28);
    size_t hossz = 50;
    SDL_Rect hova = { 0, 260, 0, 0 };
    SDL_RenderClear(renderer);
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    felirat = TTF_RenderUTF8_Solid(textFont, "Enter Your name!", feher);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    hova.x = (1280 - felirat->w) / 2;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
    input_text(state->player.Name, hossz, textBox, textBckgrnd, textText, textFont, renderer);
    TTF_CloseFont(textFont);
}

/** Kirajzolja a képernyõre az aktuális játék vezérlőpultját*/
void GameCntrllr(SDL_Renderer *renderer, SDL_Rect *gombok, Gamestate state, int charge){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    Szoveg szov;
    szov.font = TTF_OpenFont(GLOBALFONT, 18);
    if (!szov.font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    SDL_Color feher = {255, 255, 255, 255};
    szov.szin = feher;
    char str[8][50];
    strcpy(str[0], state.player.Name);
    sprintf(str[1], "%s: %d", "Score", state.player.Score);
    strcpy(str[2], "Save");
    strcpy(str[3], "Quit");
    sprintf(str[4], "%s %d", "Level ", state.level.lvlNr);
    sprintf(str[5],"%s: %d", "Tries", state.TryNr);
    strcpy(str[6], "Reset");
    strcpy(str[7], "START!");
    int x[8] = {10, 10, 1195, 1200, 300, 500, 500, 900}, y[8] = {5, 50, 5, 50, 50, 50, 5, 25};
    thickLineRGBA(renderer, 0, 80, 1280, 80, 2, 255, 255, 255, 255);
    for (int i = 0; i < 7; i++){
        szov.hova.x = x[i];
        szov.hova.y = y[i];
        strcpy(szov.str, str[i]);
        szoveg_kiir(renderer, szov, &gombok[i]);
    }
    TTF_CloseFont(szov.font);
    szov.font = TTF_OpenFont(GLOBALFONT, 26);
    if (!szov.font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    szov.hova.x = x[7];
    szov.hova.y = y[7];
    strcpy(szov.str, str[7]);
    szoveg_kiir(renderer, szov, &gombok[7]);
    TTF_CloseFont(szov.font);
    rectangleRGBA(renderer, 750, 5, 820, 75, 255, 255, 255, 255);
    if (charge < 0){
        filledCircleRGBA(renderer, 785, 40, 15, 0, 0, 255, 255);
        stringRGBA(renderer, 781, 36, "-", 255, 255, 255, 255);
    }
    else{
        filledCircleRGBA(renderer, 785, 40, 15, 255, 0, 0, 255);
        stringRGBA(renderer, 781, 36, "+", 255, 255, 255, 255);
    }
    SDL_Rect temp = {750, 5, 80, 80};
    gombok[8] = temp;
}

/**
 * Megjeleníti a képernyőre a pálya jelenlegi állását.
 */
void GameLevelDraw(SDL_Renderer *renderer, Gamestate *state){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    filledCircleRGBA(renderer, state->level.part.pos.x, state->level.part.pos.y + 81, state->level.part.radius, 225, 255, 0, 255);
    circleRGBA(renderer, state->level.part.pos.x, state->level.part.pos.y + 81, state->level.part.radius, 0, 0, 0, 255);
    stringRGBA(renderer, state->level.part.pos.x-4, state->level.part.pos.y + 77, "+", 255, 0, 0, 255);
    boxRGBA(renderer, state->level.goal.x, state->level.goal.y + 81, state->level.goal.x + state->level.goal.w, state->level.goal.y + 81 + state->level.goal.h, 255, 0, 0, 150);
    WallList *Walls = state->level.Walls;
    while (Walls != NULL){
        boxRGBA(renderer, Walls->wall.x, Walls->wall.y + 81, Walls->wall.x + Walls->wall.w, Walls->wall.y + 81 + Walls->wall.h, 220, 220, 220, 255);
        Walls = Walls->next;
    }
    ParticleList *Parts = state->level.parts;
    while (Parts != NULL){
        if (Parts->part.charge > 0){
            filledCircleRGBA(renderer, Parts->part.pos.x, Parts->part.pos.y + 81, Parts->part.radius, 255, 0, 0, 255);
            circleRGBA(renderer, Parts->part.pos.x, Parts->part.pos.y + 81, Parts->part.radius, 00, 0, 0, 255);
            stringRGBA(renderer, Parts->part.pos.x - 4, Parts->part.pos.y + 77, "+", 255, 255, 255, 255);
        }
        else {
            filledCircleRGBA(renderer, Parts->part.pos.x, Parts->part.pos.y + 81, Parts->part.radius, 0, 0, 255, 255);
            circleRGBA(renderer, Parts->part.pos.x, Parts->part.pos.y + 81, Parts->part.radius, 0, 0, 0, 255);
            stringRGBA(renderer, Parts->part.pos.x - 4, Parts->part.pos.y + 77, "-", 255, 255, 255, 255);
        }
        Parts = Parts->next;
    }



    SDL_RenderPresent(renderer);
}

/**
 * Megnézi, hogy az elért pontszámmal a játékos bekerül-e a dicsőséglistába,
 * majd ha igen, berakja.
 */
void InHoF(Player player){
    FILE *fp = fopen("HallOfFame.txt", "r");
    int HoF_Nr = HoFCount();

    Champion *HoF = (Champion*) malloc(HoF_Nr*sizeof(Champion));
    for (int i = 0; i < HoF_Nr; i++){
        char tempstr[100];
        if (fgets(tempstr, 99, fp) != NULL){
            char tempNum[50];
            Valaszt(tempstr, '\t', tempNum, HoF[i].Name);
            sscanf(tempNum, "%d", &HoF[i].Score);
        }
        else{
            strcpy(HoF[i].Name, "No Person yet");
            HoF[i].Score = 0;
        }
    }
    fclose(fp);
    int i = 0;
    while (player.Score <= HoF[i].Score){
        i++;
    }
    for (int j = HoF_Nr-1; j > i; j--){
        HoF[j] = HoF[j-1];
    }
    strcpy(HoF[i].Name, player.Name);
    HoF[i].Score = player.Score;
    fp = fopen("HallOfFame.txt", "w");
    for (int j = 0; j < HoF_Nr; j++){
        fprintf(fp, "%d\t%s\n", HoF[j].Score, HoF[j].Name);
    }
    fclose(fp);
    free(HoF);
    return;
}

/**
 * Megjeleníti a győztes feliratot, frissíti a dicsőséglistát.
 */
void Won(SDL_Renderer *renderer, Gamestate state){
    Szoveg szov;
    SDL_Rect gomb;
    szov.font = TTF_OpenFont(GLOBALFONT, 72);
    szov.hova.y = 300;
    szov.szin.r = 255;
    szov.szin.g = 255;
    szov.szin.b = 255;
    szov.szin.a = 255;
    strcpy(szov.str, "Congratulations!");
    szoveg_kiir_kozep(renderer, szov, &gomb);
    TTF_CloseFont(szov.font);
    SDL_RenderPresent(renderer);
    InHoF(state.player);
    SDL_Delay(3000);
}
