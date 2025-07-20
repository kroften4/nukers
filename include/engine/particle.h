#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "engine/vector.h"
#include "engine/engine.h"

struct particle {
    int type;
    struct vector pos;
    struct vector velocity;
    struct vector size;
    int lifetime;
};

void particle_step(struct game_state *state, int delta_time);

void add_particle(struct game_state *state, struct particle particle);

#endif

