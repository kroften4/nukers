#ifndef _LOGIC_H
#define _LOGIC_H

#include "krft/engine.h"

#define MAX_OBJECTS_AMOUNT 100

struct game_state {
    struct game_obj *objects[MAX_OBJECTS_AMOUNT];
    size_t obj_amount;
};

enum game_obj_type {
    OBJ_CAMERA,
    OBJ_PLAYER,
    OBJ_TEAMMATE,
    OBJ_ENEMY,
    OBJ_BULLET,
    OBJ_WALL,
};

struct game_obj *find_obj_by_type(struct game_state state, enum game_obj_type type);

void init_game(struct game_state *state);

#endif

