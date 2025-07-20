#include "engine/engine.h"
#include <stdlib.h>
#include "utility.h"
#include "krft/log.h"
#include "engine/particle.h"

void add_particle(struct game_state *state, struct particle particle) {
	struct particle *particle_p = malloc(sizeof(struct particle));
	*particle_p = particle;
	state->particles[state->particle_amount++] = particle_p;
}

void particle_destroy(struct game_state *state, struct particle *particle) {
    for (size_t i = 0; i < state->particle_amount; i++) {
        if (state->particles[i] == particle) {
            free(state->particles[i]);
            remove_element_by_index((void **)state->particles, &state->particle_amount, i);
            return;
        }
    }
    ERROR("Did not find the particle");
}

void particle_step(struct game_state *state, int delta_time) {
    for (size_t i = 0; i < state->particle_amount; i++) {
        struct particle *particle = state->particles[i];
        particle->lifetime -= delta_time;
        if (particle->lifetime <= 0) {
            particle_destroy(state, particle);
        }
    }
}

