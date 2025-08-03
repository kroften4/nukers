#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <stdbool.h>
#include "engine/engine.h"

void physics_step(struct game_state *state, int delta_time);

void apply_forces(struct game_state *state, int delta_time);

void add_force(struct game_state *state, entity_id_t entity,
	       struct vector force);

#endif
