#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "megjelenites.h"
#include "jatek.h"




int main(int argc, char *argv[]) {
    /* ablak létrehozása */
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("Charge", 1280, 720, &window, &renderer);
    SDL_Rect gombok[4];
    menu(renderer, gombok);


    /* varunk a kilepesre */
    SDL_Event ev;
    SDL_Point p = {ev.button.x, ev.button.y};
    while (SDL_WaitEvent(&ev) &&  ev.type != SDL_QUIT && !Button_Pressed(ev, gombok[3], p)) {

        if (Button_Pressed(ev, gombok[2], p)){
            HallOfFame(renderer);
            menu(renderer, gombok);
        }

        p.x = ev.button.x;
        p.y = ev.button.y;
    }

    /* ablak bezarasa */
    SDL_Quit();

    return 0;
}

