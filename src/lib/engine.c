#include "krft/engine.h"
#include <stddef.h>

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

bool AABB_x_overlaps_at_toi(struct AABB_bounds b1, struct AABB_bounds b2,
                            struct vector relative_vel, float toi) {
    b1.up += toi * relative_vel.y;
    b1.down += toi * relative_vel.y;
    b2.up += toi * relative_vel.y;
    b2.down += toi * relative_vel.y;
    return b1.up < b2.down || b1.down > b2.up;
}

bool AABB_y_overlaps_at_toi(struct AABB_bounds b1, struct AABB_bounds b2,
                            struct vector relative_vel, float toi) {
    b1.left += toi * relative_vel.x;
    b1.right += toi * relative_vel.x;
    b2.left += toi * relative_vel.x;
    b2.right += toi * relative_vel.x;
    return b1.left > b2.right || b1.right < b2.left;
}

float get_AABB_toi(struct game_obj obj1, struct game_obj obj2) {
    struct vector relative_vel= vector_subtract(obj1.velocity, obj2.velocity);
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

