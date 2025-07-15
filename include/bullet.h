#ifndef _BULLET_H
#define _BULLET_H

#include "krft/vector.h"
#include "logic.h"

void init_bullet(struct game_state *state, struct vector start_pos,
                 struct vector angle, float speed);

void bullet_update(struct game_obj *bullet, int delta_time);

#endif

