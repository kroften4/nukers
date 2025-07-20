#ifndef _ENGINE_H
#define _ENGINE_H

#include "engine/vector.h"
#include <stdbool.h>
#include <stddef.h>

#define MAX_OBJECTS_AMOUNT 500
#define MAX_PARTICLES_AMOUNT 2000

struct inputs {
    bool lmb;
    bool mmb;
    bool rmb;
};

enum collision_type {
    COLL_STATIC,
    COLL_DYNAMIC,
    COLL_NONE
};

struct game_state;
struct game_obj;
struct coll_info;

struct game_state {
    struct game_obj *objects[MAX_OBJECTS_AMOUNT];
    size_t obj_amount;
    struct particle *particles[MAX_PARTICLES_AMOUNT];
    size_t particle_amount;
    struct inputs inputs;
};

typedef void (*on_collision_t)(struct game_state *state, struct game_obj *self,
                         struct coll_info collision);
typedef void (*on_physics_tick_t)(struct game_state *state, struct game_obj *self);

struct game_obj {
    int id;
    int tag;
    enum collision_type coll_type;
    float speed;
    struct vector pos;
    struct vector velocity;
    struct vector size;
    on_collision_t on_collision;
    on_physics_tick_t on_physics_tick;
};

struct coll_info {
    float toi;
    struct vector normal;
    struct game_obj other;
};

struct AABB_bounds {
    float up;
    float down;
    float left;
    float right;
};

struct AABB_bounds AABB_get_bounds(struct vector pos, struct vector size);

#endif

