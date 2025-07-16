#ifndef _ENGINE_H
#define _ENGINE_H

#include "vector.h"
#include <stdbool.h>
#include <stddef.h>

#define MAX_OBJECTS_AMOUNT 100

struct coll_info;
struct game_state;

enum collision_type {
    COLL_STATIC,
    COLL_DYNAMIC,
    COLL_NONE
};

struct inputs {
    bool lmb;
};

struct game_obj {
    int id;
    int tag;
    enum collision_type coll_type;
    float speed;
    struct vector pos;
    struct vector velocity;
    struct vector size;
    void (*on_collision)(struct game_state *state, struct game_obj *self,
                         struct coll_info collision);
    void (*on_physics_tick)(struct game_state *state, struct game_obj *self);
};

struct coll_info {
    float toi;
    struct vector normal;
    struct game_obj other;
};

struct game_state {
    struct game_obj *objects[MAX_OBJECTS_AMOUNT];
    struct inputs inputs;
    size_t obj_amount;
};

void object_destroy(struct game_state *state, struct game_obj *object);

float resolve_first_toi(struct game_state *state, int delta_time);

void physics_step(struct game_state *state, int delta_time);

struct game_obj linear_move(struct game_obj obj, int delta_time);

bool get_first_collision(struct coll_info *collisions, size_t amount,
                         struct coll_info *result);

struct AABB_bounds {
    float up;
    float down;
    float left;
    float right;
};

struct AABB_bounds AABB_get_bounds(struct game_obj obj);

bool AABB_is_overlapping(struct game_obj first, struct game_obj second);

struct vector linear_interpolate(struct vector pos, struct vector pos_next,
                                 float toi);

bool is_valid_toi(float toi);

void check_AABB_collision(struct game_obj obj1, struct game_obj obj2,
                          float *result_toi, struct vector *normal1,
                          int delta_time);

#endif

