#include "bullet.h"
#include "input.h"
#include "player.h"
#include "krft/engine.h"
#include "krft/vector.h"
#include "logic.h"
#include <SDL3/SDL_events.h>
#include <stdlib.h>

void player_on_collision(struct game_state *state, struct game_obj *self,
                         struct coll_info collision) {
    (void)state;
    (void)collision;
    if (vector_dot(self->velocity, collision.normal) < 0) {
        // rotate 90
        struct vector surface = {-collision.normal.y, collision.normal.x};
        // project
        float dot = vector_dot(self->velocity, surface);
        self->velocity = vector_multiply(surface, dot);
    }
}

void player_input_respond(struct game_obj *player) {
    player->velocity.x = get_input_horizontal();
    player->velocity.y = get_input_vertical();
    player->velocity = vector_normalize(player->velocity);
    player->velocity = vector_multiply(player->velocity,
                                       player->speed);
}

void player_shoot(struct game_state *state, struct game_obj player,
                  struct game_obj camera) {
    struct vector bullet_dir = get_mouse_direction(player.pos, camera);
    init_bullet(state, player.pos, bullet_dir, 0.5);
}

void player_update(struct game_state *state, struct game_obj *player) {
    struct game_obj *camera = find_obj_by_tag(*state, OBJ_CAMERA);
    player_input_respond(player);
    if (state->inputs.lmb)
        player_shoot(state, *player, *camera);
}

void init_player(struct game_state *state) {
    struct game_obj *player = malloc(sizeof(struct game_obj));
    player->on_collision = player_on_collision;
    player->on_physics_tick = player_update;
    player->coll_type = COLL_DYNAMIC;
    player->tag = OBJ_PLAYER;
    player->pos.x = 200;
    player->pos.y = 200;

    player->size.x = 20;
    player->size.y = 20;

    player->speed = 0.5;

    state->objects[state->obj_amount++] = player;
}

