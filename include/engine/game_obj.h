#ifndef _GAME_OBJ_H
#define _GAME_OBJ_H

#include "engine/engine.h"

void object_destroy(struct game_state *state, struct game_obj *object);

void add_object_to_state(struct game_obj obj, struct game_state *state);

#endif

