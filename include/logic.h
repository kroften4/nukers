#ifndef _LOGIC_H
#define _LOGIC_H

#include "krft/engine.h"

enum game_obj_tag {
    OBJ_CAMERA,
    OBJ_PLAYER,
    OBJ_TEAMMATE,
    OBJ_ENEMY,
    OBJ_BULLET,
    OBJ_WALL,
};

enum particle_type {
    PAR_EXPLOSION
};

struct game_obj *find_obj_by_tag(struct game_state state, enum game_obj_tag type);

void init_game(struct game_state *state);

#endif

