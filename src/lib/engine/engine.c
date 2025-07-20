#include "engine/engine.h"
#include "engine/game_obj.h"
#include "engine/input.h"
#include "krft/log.h"
#include "engine/vector.h"
#include <stddef.h>
#include <stdlib.h>
#include "utility.h"

struct AABB_bounds AABB_get_bounds(struct vector pos, struct vector size) {
    struct AABB_bounds boundaries = {
        .up = pos.y + size.y / 2,
        .down = pos.y - size.y / 2,
        .left = pos.x - size.x / 2,
        .right = pos.x + size.x / 2
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

