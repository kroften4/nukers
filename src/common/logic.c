#include "logic/logic.h"
#include "engine/engine.h"
#include "logic/player.h"
#include <stddef.h>
#include <stdlib.h>

static void init_camera(struct game_state *state) {
    struct game_obj *camera = malloc(sizeof(struct game_obj));
    camera->on_physics_tick = NULL;
    camera->coll_type = COLL_NONE;
    camera->on_collision = NULL;
    camera->tag = OBJ_CAMERA;
    camera->pos.x = 0;
    camera->pos.y = 0;

    camera->size.x = 640;
    camera->size.y = 480;

    state->objects[state->obj_amount++] = camera;
}

static struct game_obj *init_wall_tile(struct game_state *state, size_t x, size_t y) {
    struct game_obj *tile = malloc(sizeof(struct game_obj));
    tile->on_physics_tick = NULL;
    tile->coll_type = COLL_STATIC;
    tile->on_collision = NULL;
    tile->tag = OBJ_WALL;
    int scale = 50;
    tile->pos.x = x * scale;
    tile->pos.y = y * scale;

    tile->size.x = scale;
    tile->size.y = scale;

    state->objects[state->obj_amount++] = tile;
    return tile;
}

static void init_walls(struct game_state *state) {
    bool walls[10][10] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    size_t w = 10;
    size_t h = 10;
    struct game_obj *wall_grid[w][h];
    for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
            if (walls[y][x])
                wall_grid[y][x] = init_wall_tile(state, x, y);
        }
    }
}

void init_game(struct game_state *state) {
    init_player(state);
    init_camera(state);
    init_walls(state);
}
