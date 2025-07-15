#include "logic.h"
#include "krft/engine.h"
#include "krft/vector.h"
#include "player.h"
#include <stddef.h>
#include <stdlib.h>

struct game_obj *find_obj_by_type(struct game_state state, enum game_obj_type type) {
    for (size_t i = 0; i < state.obj_amount; i++) {
        if ((enum game_obj_type)state.objects[i]->type == type)
            return state.objects[i];
    }
    return NULL;
}

void init_camera(struct game_state *state) {
    struct game_obj *camera = malloc(sizeof(struct game_obj));
    camera->type = OBJ_CAMERA;
    camera->pos.x = 0;
    camera->pos.y = 0;

    camera->size.x = 640;
    camera->size.y = 480;

    state->objects[state->obj_amount++] = camera;
}

void init_game(struct game_state *state) {
    init_player(state);
    init_camera(state);
}

void collide_state_objects(struct game_state state) {
}
