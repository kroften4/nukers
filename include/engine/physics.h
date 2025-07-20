#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <stdbool.h>
#include "engine/engine.h"

void physics_step(struct game_state *state, int delta_time);

#endif

