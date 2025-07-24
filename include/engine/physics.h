#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <stdbool.h>
#include "engine/engine.h"

typedef void (*on_physics_step_t)(struct game_state *state, int delta_time);
typedef void (*on_physics_end_t)(struct game_state *state, int delta_time);

void physics_step(struct game_state *state, int delta_time, on_physics_step_t on_physics_step, on_physics_end_t on_physics_end);

#endif

