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
 * Hozzáad egy meglévõ részecskelistához egy megadott részecskét.
 * @param first a lista elejére mutató pointer
 * @param data a részecske adatai Particle struktúrában
 * @return az megváltoztatott listára mutató pointer
 */
ParticleList *addPart(ParticleList *first, Particle data){
    ParticleList *newElem = (ParticleList*) malloc(sizeof(ParticleList));
    if (newElem == NULL){
        SDL_Log("Memory could not be allocated!\n");
        exit(1);
    }
    newElem->part = data;
    newElem->next = first;
    first = newElem;
    return first;
}

/**
 * Hozzáad egy meglévõ fallistához egy megadott falat.
 * @param first a lista elejére mutató pointer
 * @param data a al adatait tartalmazó SDL_Rect
 * @return az megváltoztatott listára mutató pointer
 */
WallList *addWall(WallList *first, SDL_Rect data){
    WallList *newElem = (WallList*) malloc(sizeof(WallList));
    if (newElem == NULL){
        SDL_Log("Memory could not be allocated!\n");
        exit(1);
    }
    newElem->wall = data;
    newElem->next = first;
    first = newElem;
    return first;
}

/**
 * létrehoz egy új részecskét.
 *
 */
Particle newPart(int x, int y, int charge){
    Particle particle;
    particle.acc.vx = 0;
    particle.acc.vy = 0;
    particle.charge = charge;
    particle.mass = 1;
    particle.pos.x = x;
    particle.pos.y = y;
    particle.radius = 10;
    particle.vel.vx = 0;
    particle.vel.vy = 0;
    return particle;
}

/**
 * Felszabadítja a meglévõ részecskelista összes elemét, a pointerjét NULL-ra állítja.
 * @param first a lista elejére mutató pointer
 *
 */
void freePart(ParticleList **first){
    ParticleList *temp;
    while (*first != NULL){
        temp = (*first)->next;
        free(*first);
        (*first) = temp;
    }
    first = NULL;
}

/**
 * Felszabadítja a meglévõ fallista összes elemét, a pointerjét NULL-ra állítja.
 * @param first a lista elejére mutató pointer
 */
void freeWall(WallList **first){
    WallList *temp;
    while (*first != NULL){
        temp = (*first)->next;
        free(*first);
        (*first) = temp;
    }
    first = NULL;
}
