#ifndef _BULLET_H
#define _BULLET_H

#include "engine/vector.h"
#include "engine/engine.h"

void init_bullet(struct game_state *state, struct vector start_pos,
                 struct vector angle, float speed);

void bullet_on_update(struct game_state *state, entity_id_t self);

#endif

