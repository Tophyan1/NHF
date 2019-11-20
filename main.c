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
#include "lista.h"




int main(int argc, char *argv[]) {
    /* ablak létrehozása */
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("Charge!", 1280, 720, &window, &renderer);
    SDL_Rect gombok[4];
    menu(renderer, gombok);



    SDL_Event ev;
    SDL_Point p = {ev.button.x, ev.button.y};
    while (SDL_WaitEvent(&ev) &&  ev.type != SDL_QUIT && !Button_Pressed(ev, gombok[3], p)) {

        if (Button_Pressed(ev, gombok[2], p)){
            HallOfFame(renderer);
            menu(renderer, gombok);
        }
        else if (Button_Pressed(ev, gombok[0], p)){
            Gamestate state;
            state.player.Score = 0;
            state.TryNr = 0;
            FILE* fp = fopen("Levels\\Level_1.txt", "r");
            if (fp == NULL){
                perror("File could not be opened!\n");
                exit(1);
            }
            state.level = GetLevel(fp);
            fclose(fp);
            GetName(&state, renderer);
            Game(renderer, &state);
          //  printf("\n%d %d %d %d\n", gombok[3].x, gombok[3].y, gombok[3].w, gombok[3].h);
            menu(renderer, gombok);
        }
        else if (Button_Pressed(ev, gombok[1], p)){
                Gamestate state;
                state.level.part.radius = 10;
                state.level.part.mass = 1;
                state.level.lvlNr = 0;
                state.level.part.charge = 1;
                state.level.part.acc.vy = 0;
                state.level.part.acc.vx = 0;
            if (LoadGame(&state)){
                Game(renderer, &state);
            }
            menu(renderer, gombok);
        }
        p.x = ev.button.x;
        p.y = ev.button.y;
    }

    /* ablak bezarasa */
    SDL_Quit();

    return 0;
}

