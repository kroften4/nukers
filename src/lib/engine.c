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

float get_AABB_toi(struct game_obj obj1, struct game_obj obj2) {
    struct vector relative_vel = vector_subtract(obj1.velocity, obj2.velocity);
    struct AABB_bounds obj1_bounds = AABB_get_bounds(obj1);
    struct AABB_bounds obj2_bounds = AABB_get_bounds(obj2);

    float toi_min = 99;

    float toi_right = (obj2_bounds.right - obj1_bounds.left) / relative_vel.x;
    bool is_valid_toi = AABB_y_overlaps_at_toi(obj1_bounds, obj2_bounds,
                                               relative_vel, toi_right);
    if (toi_right < toi_min && is_valid_toi)
        toi_min = toi_right;

    float toi_left = (obj2_bounds.left - obj1_bounds.right) / relative_vel.x;
    is_valid_toi = AABB_y_overlaps_at_toi(obj1_bounds, obj2_bounds,
                                          relative_vel, toi_left);
    if (toi_left < toi_min && is_valid_toi)
        toi_min = toi_left;

    float toi_top = (obj2_bounds.up - obj1_bounds.down) / relative_vel.y;
    is_valid_toi = AABB_x_overlaps_at_toi(obj1_bounds, obj2_bounds,
                                          relative_vel, toi_top);
    if (toi_top < toi_min && is_valid_toi)
        toi_min = toi_top;

    float toi_bottom = (obj2_bounds.down - obj1_bounds.up) / relative_vel.y;
    is_valid_toi = AABB_x_overlaps_at_toi(obj1_bounds, obj2_bounds,
                                          relative_vel, toi_bottom);
    if (toi_bottom < toi_min && is_valid_toi)
        toi_min = toi_bottom;

    return toi_min;
}

void resolve_collision(struct game_state *state, float toi, struct game_obj *obj1, struct game_obj *obj2, struct vector normal1) {
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

void collide_state_objects(struct game_state *state) {
    struct game_obj *coll_obj1 = NULL;
    struct game_obj *coll_obj2 = NULL;
    float toi_min = 99;
    for (size_t i = 0; i < state->obj_amount - 1; i++) {
        for (size_t j = i + 1; j < state->obj_amount; j++) {
            struct game_obj *obj1 = state->objects[i];
            struct game_obj *obj2 = state->objects[j];
            if (obj1->coll_type == COLL_STATIC && obj2->coll_type == COLL_STATIC) 
                continue;
            if (obj1->coll_type == COLL_NONE || obj2->coll_type == COLL_NONE)
                continue;

            float toi = get_AABB_toi(*obj1, *obj2);
            if (is_valid_toi(toi) && toi < toi_min) {
                toi_min = toi;
                coll_obj1 = obj1;
                coll_obj2 = obj2;
            }
        }
    }
    if (toi_min == 99)
        return;
    LOGF("min toi %f", toi_min);
    // TODO: calc normal
    struct vector normal1 = {0, 0};
    resolve_collision(state, toi_min, coll_obj1, coll_obj2, normal1);
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

