#include "krft/engine.h"
#include "krft/log.h"
#include "krft/vector.h"
#include <stddef.h>
#include <stdlib.h>

struct game_obj linear_move(struct game_obj obj, int delta_time) {
    struct vector corrected_vel = vector_multiply(obj.velocity, delta_time);
    obj.pos = vector_add(obj.pos, corrected_vel);
    return obj;
}

struct AABB_bounds AABB_get_bounds(struct game_obj obj) {
    struct AABB_bounds boundaries = {
        .up = obj.pos.y + obj.size.y / 2,
        .down = obj.pos.y - obj.size.y / 2,
        .left = obj.pos.x - obj.size.x / 2,
        .right = obj.pos.x + obj.size.x / 2
    };
    return boundaries;
}

struct vector linear_interpolate(struct vector pos, struct vector pos_next,
                                 float toi) {
    struct vector pos_interpolated = {0, 0};
    pos_interpolated.x = toi * (pos_next.x - pos.x) + pos.x;
    pos_interpolated.y = toi * (pos_next.y - pos.y) + pos.y;
    return pos_interpolated;
}

bool is_valid_toi(float toi) {
    return 0.0f < toi && toi <= 1.0f;
}

bool AABB_is_overlapping(struct game_obj first, struct game_obj second) {
    struct AABB_bounds bounds1 = AABB_get_bounds(first);
    struct AABB_bounds bounds2 = AABB_get_bounds(second);
    if (bounds1.down > bounds2.up)
        return true;
    if (bounds1.up < bounds2.down)
        return true;
    if (bounds1.left > bounds2.right)
        return true;
    if (bounds1.right < bounds2.left)
        return true;
    return false;
}

bool get_first_collision(struct coll_info *collisions, size_t amount, struct coll_info *result) {
    result->toi = 99;
    for (size_t i = 0; i < amount; i++) {
        if (is_valid_toi(collisions[i].toi) && collisions[i].toi < result->toi)
            *result = collisions[i];
    }
    return result->toi != 99;
}

bool AABB_y_overlaps_at_toi(struct AABB_bounds b1, struct AABB_bounds b2,
                            struct vector relative_vel, float toi) {
    b1.up += toi * relative_vel.y;
    b1.down += toi * relative_vel.y;
    return b1.down < b2.up && b1.up > b2.down;
}

bool AABB_x_overlaps_at_toi(struct AABB_bounds b1, struct AABB_bounds b2,
                            struct vector relative_vel, float toi) {
    b1.left += toi * relative_vel.x;
    b1.right += toi * relative_vel.x;
    return b1.left < b2.right && b1.right > b2.left;
}

void check_AABB_collision(struct game_obj obj1, struct game_obj obj2,
                          float *result_toi, struct vector *normal1,
                          int delta_time) {
    struct vector relative_vel = vector_subtract(obj1.velocity, obj2.velocity);
    relative_vel = vector_multiply(relative_vel, delta_time);
    struct AABB_bounds obj1_bounds = AABB_get_bounds(obj1);
    struct AABB_bounds obj2_bounds = AABB_get_bounds(obj2);

    float toi_min = 99;
    *normal1 = VEC_ZERO;

    float toi_right = (obj2_bounds.right - obj1_bounds.left) / relative_vel.x;
    bool is_overlapping = AABB_y_overlaps_at_toi(obj1_bounds, obj2_bounds,
                                               relative_vel, toi_right);
    if (toi_right < toi_min && is_overlapping)
        toi_min = toi_right;

    float toi_left = (obj2_bounds.left - obj1_bounds.right) / relative_vel.x;
    is_overlapping = AABB_y_overlaps_at_toi(obj1_bounds, obj2_bounds,
                                          relative_vel, toi_left);
    if (toi_left < toi_min && is_overlapping)
        toi_min = toi_left;

    float toi_top = (obj2_bounds.up - obj1_bounds.down) / relative_vel.y;
    is_overlapping = AABB_x_overlaps_at_toi(obj1_bounds, obj2_bounds,
                                          relative_vel, toi_top);
    if (toi_top < toi_min && is_overlapping)
        toi_min = toi_top;

    float toi_bottom = (obj2_bounds.down - obj1_bounds.up) / relative_vel.y;
    is_overlapping = AABB_x_overlaps_at_toi(obj1_bounds, obj2_bounds,
                                          relative_vel, toi_bottom);
    if (toi_bottom < toi_min && is_overlapping)
        toi_min = toi_bottom;

    if (toi_min == toi_top)
        *normal1 = vector_add(*normal1, VEC_UP);
    if (toi_min == toi_bottom)
        *normal1 = vector_add(*normal1, VEC_DOWN);
    if (toi_min == toi_left)
        *normal1 = vector_add(*normal1, VEC_LEFT);
    if (toi_min == toi_right)
        *normal1 = vector_add(*normal1, VEC_RIGHT);

    *result_toi = toi_min;
}

void resolve_collision(struct game_state *state, float toi,
                       struct game_obj *obj1, struct game_obj *obj2,
                       struct vector normal1) {
    struct coll_info collision1 = {
        .toi = toi,
        .normal = normal1,
        .other = *obj2
    };
    struct coll_info collision2 = {
        .toi = toi,
        .normal = vector_multiply(normal1, -1),
        .other = *obj1
    };
    if (obj1->on_collision != NULL)
        obj1->on_collision(state, obj1, collision1);
    if (obj2->on_collision != NULL)
        obj2->on_collision(state, obj2, collision2);
}

void move_state_objects(struct game_state *state, int delta_time, float toi) {
    for (size_t i = 0; i < state->obj_amount; i++) {
        struct game_obj *obj = state->objects[i];
        struct vector corrected_vel = vector_multiply(obj->velocity, delta_time * toi);
        if (corrected_vel.x != 0 || corrected_vel.y != 0)
            LOGF("vel (%f %f) -> (%f %f), dt %d, toi %f",
                 obj->velocity.x, obj->velocity.y, corrected_vel.x,
                 corrected_vel.y, delta_time, toi);
        obj->pos = vector_add(obj->pos, corrected_vel);
    }
}

void physics_tick_notify(struct game_state *state) {
    for (size_t i = 0; i < state->obj_amount; i++) {
        struct game_obj *obj = state->objects[i];
        if (obj->on_physics_tick != NULL)
            obj->on_physics_tick(state, obj);
    }
}

void physics_step(struct game_state *state, int delta_time) {
    physics_tick_notify(state);
    int cap = 100;
    int iterations = 0;
    int passed_time = 0;
    while (passed_time < delta_time && iterations < cap) {
        int remaining_time = delta_time - passed_time;
        passed_time += remaining_time * resolve_first_toi(state, remaining_time);
        iterations++;
    }
    if (iterations == cap) {
        ERROR("reached 100 collision iterations");
    }
}

float resolve_first_toi(struct game_state *state, int delta_time) {
    float toi_min = 99;

    struct collision {
        struct game_obj *obj1;
        struct game_obj *obj2;
        struct vector normal1;
        float toi;
    };
    struct collision collisions[MAX_OBJECTS_AMOUNT];
    size_t coll_amount = 0;

    for (size_t i = 0; i < state->obj_amount - 1; i++) {
        for (size_t j = i + 1; j < state->obj_amount; j++) {
            struct game_obj *obj1 = state->objects[i];
            struct game_obj *obj2 = state->objects[j];
            if (obj1->coll_type == COLL_STATIC && obj2->coll_type == COLL_STATIC) 
                continue;
            if (obj1->coll_type == COLL_NONE || obj2->coll_type == COLL_NONE)
                continue;

            float toi;
            struct vector normal;
            check_AABB_collision(*obj1, *obj2, &toi, &normal, delta_time);
            if (toi == 0) {
                resolve_collision(state, toi, obj1, obj2, normal);
            }
            if (!is_valid_toi(toi))
                continue;

            if (toi == toi_min) {
                collisions[coll_amount].normal1 = normal;
                collisions[coll_amount].toi = toi;
                collisions[coll_amount].obj1 = obj1;
                collisions[coll_amount].obj2 = obj2;
                coll_amount++;
            }
            if (toi < toi_min) {
                coll_amount = 1;
                collisions[0].normal1 = normal;
                collisions[0].toi = toi;
                collisions[0].obj1 = obj1;
                collisions[0].obj2 = obj2;
                toi_min = toi;
            }
        }
    }
    if (!is_valid_toi(toi_min)) {
        move_state_objects(state, delta_time, 1);
        return 1;
    }

    LOGF("dt %d, toi %f, coll_amount %zu", delta_time, toi_min, coll_amount);
    move_state_objects(state, delta_time, toi_min);
    for (size_t i = 0; i < coll_amount; i++) {
        resolve_collision(state, collisions[i].toi, collisions[i].obj1,
                          collisions[i].obj2, collisions[i].normal1);
    }

    return toi_min;
}

void remove_element_by_index(struct game_obj *objects[], size_t *n, size_t index) {
    *n -= 1;
    for (size_t i = index; i < *n; i++) {
        objects[i] = objects[i + 1];
    }
}

void object_destroy(struct game_state *state, struct game_obj *object) {
    for (size_t i = 0; i < state->obj_amount; i++) {
        if (state->objects[i] == object) {
            free(state->objects[i]);
            remove_element_by_index(state->objects, &state->obj_amount, i);
            return;
        }
    }
    ERROR("Did not find the object");
}

