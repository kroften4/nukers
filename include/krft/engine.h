#ifndef _ENGINE_H
#define _ENGINE_H

#include "vector.h"
#include <stdbool.h>
#include <stddef.h>

struct game_obj {
    int id;
    int type;
    float speed;
    struct vector pos;
    struct vector velocity;
    struct vector size;
};

struct game_obj linear_move(struct game_obj obj, int delta_time);

struct coll_info {
    float toi;
    struct vector normal;
    struct vector pos;
};

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
                                 float delta_time_normalized);

bool is_valid_toi(float toi);

float get_AABB_toi(struct game_obj obj1, struct game_obj obj2);

#endif

