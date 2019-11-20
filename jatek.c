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

/**
 * Visszatér a dicsőséglistában tárolt személyek számával.
 */
int HoFCount(){
    return 11;
}

/**
 * Megnézi, hogy egy részecske ütközött-e egy fallal, vagy elérte-e a célmezõt
 * @param part a tesztelendõ részecske
 * @param Wall a fal vagy mezõ
 * @return IGAZ, ha nekiütközött a falnak, vagy elérte a mezõt, különben HAMIS
 */
bool TestCollision(Particle part, SDL_Rect Wall){
    SDL_Rect CollField = {
                    Wall.x - part.radius,
                    Wall.y - part.radius,
                    Wall.w + 2*part.radius,
                    Wall.h + 2*part.radius
    };
    return SDL_PointInRect(&part.pos, &CollField);
}

/**
 * Kiszámolja két részecske közötti elektromágneses erõt.
 * @param p1 a részecske, ami az erõt okozza
 * @param p2 a részecske, amire az erõ hat
 * @return a p1 által p2-re ható erõ vektora
 */
Vector Force(Particle p1, Particle p2){
    double phi = atan2(-(p2.pos.y-p1.pos.y), (p2.pos.x-p1.pos.x));
    double mag = 600000*p1.charge*p2.charge / ((p2.pos.x-p1.pos.x)*(p2.pos.x-p1.pos.x)+(p2.pos.y-p1.pos.y)*(p2.pos.y-p1.pos.y));
  //  printf("%f\n", mag);
    Vector v = {-mag*cos(phi), mag*sin(phi)};
    return v;
}

/**
 * Kiszámolja a következõ idõegység lejárta utáni helyét egy részecskének.
 */
SDL_Point NextPos(SDL_Point p, Vector v, int t){
    p.x += v.vx * t/500;
    p.y += v.vy * t/500;
    return p;
}

/**
 * Kiolvassa a pálya adatait egy fájlból.
 * @param fp a pálya adatait tartalmazó fájl
 * @return a pálya, struktúrában
 */
Level GetLevel(FILE *fp){
    Level level;
    level.part.radius = 10;
    level.part.mass = 1;
    level.partNr = 0;
    level.part.charge = 1;
    level.part.acc.vx = 0;
    level.part.acc.vy = 0;
    level.Walls = NULL;
    level.parts = NULL;
    fscanf(fp, "%d\n%d %d\n%lf %lf\n%d %d %d %d\n", &level.lvlNr, &level.part.pos.x, &level.part.pos.y, &level.part.vel.vx, &level.part.vel.vy, &level.goal.x, &level.goal.y, &level.goal.w, &level.goal.h);
    level.orPos = level.part.pos;
    level.orVel = level.part.vel;
    char str[51];
    int i = 0;
    SDL_Rect Wall;
    while (fgets(str, 50, fp) != 0){
        sscanf(str, "%d %d %d %d ", &Wall.x, &Wall.y, &Wall.w, &Wall.h);
        level.Walls = addWall(level.Walls, Wall);
        i++;
    }
    level.parts = NULL;
    return level;
}

/**
 * Elemnti a jelenlegi játékállást egy fájlba.
 * A fájl relatív helye: SaveGame/current_save.txt
 *
 *
 */
void SaveGame(Gamestate state){
    FILE* fp = fopen("SaveGame\\current_save.txt", "w");
    if (fp == NULL){
        SDL_Log("Save file could not be created!\n");
        exit(1);
    }
    fprintf(fp, "%s\t%d\n", state.player.Name, state.player.Score);
    fprintf(fp, "%d\n", state.TryNr);
    fprintf(fp, "%d\n", state.level.lvlNr);
    fprintf(fp, "%d %d %d %d\n", state.level.goal.x, state.level.goal.y, state.level.goal.w, state.level.goal.h);
    fprintf(fp, "%d %d %f %f\n", state.level.part.pos.x, state.level.part.pos.y, state.level.part.vel.vx, state.level.part.vel.vy);
    WallList *Wall = state.level.Walls;
    while (Wall != NULL){
        fprintf(fp, "%d %d %d %d ", Wall->wall.x, Wall->wall.y, Wall->wall.w, Wall->wall.h);
        Wall = Wall->next;
    }
    fprintf(fp, "-1 -1 -1 -1 \n");
    ParticleList *Parts = state.level.parts;
    while (Parts != NULL){
        fprintf(fp, "%d %d %d ", Parts->part.pos.x, Parts->part.pos.y, Parts->part.charge);
        Parts = Parts->next;
    }
    fprintf(fp, "-1 -1 0 ");
    fclose(fp);
}

/**/
bool LoadGame(Gamestate *state){
    FILE* fp = fopen("SaveGame\\current_save.txt", "r");
    if (fp == NULL){
        return false;
    }
    char str[100], tempNum[10];
    fgets(str, 99, fp);
    Valaszt(str, '\t', state->player.Name, tempNum);
    sscanf(tempNum, "%d", &state->player.Score);
    fscanf(fp, "%d\n", &state->TryNr);
    fscanf(fp, "%d\n", &state->level.lvlNr);
    fscanf(fp, "%d %d %d %d\n", &state->level.goal.x, &state->level.goal.y, &state->level.goal.w, &state->level.goal.h);
    fscanf(fp, "%d %d %lf %lf\n", &state->level.part.pos.x, &state->level.part.pos.y, &state->level.part.vel.vx, &state->level.part.vel.vy);
    state->level.orPos = state->level.part.pos;
    state->level.orVel = state->level.part.vel;
    SDL_Rect Wall = {0, 0, 0, 0};
    state->level.Walls = NULL;
    while ((Wall.x != -1) || (Wall.y != -1) || (Wall.w != -1) || (Wall.h != -1)){
        fscanf(fp, "%d %d %d %d ", &Wall.x, &Wall.y, &Wall.w, &Wall.h);
        state->level.Walls = addWall(state->level.Walls, Wall);
    }
    fscanf(fp, "\n");
    int x, y, charge;
    fscanf(fp, "%d %d %d", &x, &y, &charge);
    state->level.parts = NULL;
    while ((x != -1) || (y != -1) || (charge != 0)){
        Particle newParticle = newPart(x, y, charge);
        state->level.partNr++;
        state->level.parts = addPart(state->level.parts, newParticle);
        fscanf(fp, "%d %d %d", &x, &y, &charge);
    }
    fclose(fp);
    return true;
}

/**
 * ez a fuggveny hivodik meg az idozito altal.
 * betesz a feldolgozando esemenyek koze (push) egy felhasznaloi esemenyt */
Uint32 idozit(Uint32 ms, void *param) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;   /* ujabb varakozas */
}

/**
 * Összegzi a listában összes részecske által kifejtett erőt a részecskére.
 *
 *
 */
Vector SumForce(Particle part, ParticleList *first){
    Vector F = {0, 0}, Ftemp = {0, 0};
    ParticleList *mover = first;
    while (mover != NULL){
        Ftemp = Force(part, mover->part);
        F.vx += Ftemp.vx;
        F.vy += Ftemp.vy;
        mover = mover->next;
    }
    return F;
}



/**
 * Az indítás gomb utáni történéseket vezérli:
 * A részecske mozgását, ütközéseket, pontszámítást.
 */
void GamePlay(SDL_Renderer *renderer, Gamestate *state, SDL_Rect *gombok, SDL_Rect field, int charge, bool *win){
    SDL_Event ev;
    int t = 20;
    SDL_Rect Field = {field.x+state->level.part.radius, field.y+state->level.part.radius-80, field.w-2*state->level.part.radius, field.h-2*state->level.part.radius};
    SDL_TimerID id = SDL_AddTimer(t, idozit , NULL);
    while (SDL_WaitEvent(&ev) && !*win){
        if (ev.type == SDL_USEREVENT){
            Vector F = SumForce(state->level.part, state->level.parts);
            state->level.part.acc.vx = F.vx/state->level.part.mass;
            state->level.part.acc.vy = F.vy/state->level.part.mass;
            state->level.part.vel.vx = state->level.part.vel.vx + state->level.part.acc.vx*t/500;
            state->level.part.vel.vy = state->level.part.vel.vy + state->level.part.acc.vy*t/500;
            state->level.part.pos = NextPos(state->level.part.pos, state->level.part.vel, t);
            GameCntrllr(renderer, gombok, *state, charge);
            GameLevelDraw(renderer, state);
            WallList *Wall = state->level.Walls;
            while (Wall != NULL){
                if (TestCollision(state->level.part, Wall->wall)){
                    while (SDL_WaitEvent(&ev) && ev.type != SDL_MOUSEBUTTONDOWN){

                    }
                    state->level.part.pos = state->level.orPos;
                    state->level.part.vel = state->level.orVel;
                    state->level.part.acc.vx = 0;
                    state->level.part.acc.vy = 0;
                    GameCntrllr(renderer, gombok, *state, charge);
                    GameLevelDraw(renderer, state);
                    SDL_RemoveTimer(id);
                    return;
                }
                Wall = Wall->next;
            }
            if (!SDL_PointInRect(&state->level.part.pos, &Field)){
                    while (SDL_WaitEvent(&ev) && ev.type != SDL_MOUSEBUTTONDOWN){

                    }
                    state->level.part.pos = state->level.orPos;
                    state->level.part.vel = state->level.orVel;
                    state->level.part.acc.vx = 0;
                    state->level.part.acc.vy = 0;
                    GameCntrllr(renderer, gombok, *state, charge);
                    GameLevelDraw(renderer, state);
                    SDL_RemoveTimer(id);
                    return;
            }
            if (TestCollision(state->level.part, state->level.goal)){
                state->player.Score += (5/state->TryNr)*(3000+(21-state->level.partNr)*500);
                GameCntrllr(renderer, gombok, *state, charge);
                GameLevelDraw(renderer, state);
                freePart(&state->level.parts);
                freeWall(&state->level.Walls);
                state->level.lvlNr += 1;
                if (state->level.lvlNr > 5){
                    *win = true;
                    SDL_RemoveTimer(id);
                    return;
                }
                char str[50];
                sprintf(str, "%s%d%s", "Levels\\Level_", state->level.lvlNr, ".txt");
                FILE* fp = fopen(str, "r");
                state->level = GetLevel(fp);
                fclose(fp);
                state->TryNr = 0;
                SDL_Delay(500);
                GameCntrllr(renderer, gombok, *state, charge);
                GameLevelDraw(renderer, state);
                break;
            }
        }
    }
    SDL_RemoveTimer(id);
}

/**
 * A játéktörzs event loopja, az átláthatóság kedvéért.
 *
 *
 */
void GameEventLoop(SDL_Renderer *renderer, Gamestate *state, SDL_Rect *gombok, int *charge, bool *win){
    SDL_Event ev;
    SDL_Point p;

    SDL_Rect field = {0, 80, 1280, 640};
    while (SDL_WaitEvent(&ev)){
        if (ev.type == SDL_MOUSEBUTTONDOWN || *win){
            p.x = ev.button.x;
            p.y = ev.button.y;
            if (Button_Pressed(ev, gombok[3], p) || *win){
                freePart(&state->level.parts);
                freeWall(&state->level.Walls);
                return;
            }

            if (Button_Pressed(ev, gombok[7], p)){
                    state->TryNr++;
                    GamePlay(renderer, state, gombok, field, *charge, win);
            }
            else  if (Button_Pressed(ev, gombok[8], p)){
                *charge *= -1;
                GameCntrllr(renderer, gombok, *state, *charge);
                GameLevelDraw(renderer, state);
            }
            else if (Button_Pressed(ev, gombok[2], p)){
                SaveGame(*state);
            }
            else if (Button_Pressed(ev, gombok[6], p)){
                freePart(&state->level.parts);
                state->level.partNr = 0;
                GameCntrllr(renderer, gombok, *state, *charge);
                GameLevelDraw(renderer, state);
            }
            else if (Button_Pressed(ev, field, p)){
                Particle new = newPart(ev.button.x, ev.button.y - 80, *charge);
                state->level.parts = addPart(state->level.parts, new);
                state->level.partNr++;
                GameCntrllr(renderer, gombok, *state, *charge);
                GameLevelDraw(renderer, state);

            }
        }

    }
}

/**
 * A láték futását intézi
 *
 */
void Game(SDL_Renderer *renderer, Gamestate *state){
    SDL_Rect gombok[10];
    int charge = -1;
    bool win = false;
    GameCntrllr(renderer, gombok, *state, charge);
    GameLevelDraw(renderer, state);
    GameEventLoop(renderer, state, gombok, &charge, &win);
    if (win){
        Won(renderer, *state);
    }

    return;
}
