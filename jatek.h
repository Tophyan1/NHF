#ifndef JATEK_H
#define JATEK_H

typedef struct Particle{
    SDL_Point hely;
    int radius;
    int charge;
    int mass;
}particle;

typedef struct ParticleList{
    Particle part;
    struct Particle *next;
}ParticleList;


#endif // JATEK_H
