#include "bullet.h"
#include "input.h"
#include "player.h"
#include "krft/engine.h"
#include "krft/vector.h"
#include "logic.h"
#include <SDL3/SDL_events.h>
#include <stdlib.h>

void init_player(struct game_state *state) {
    struct game_obj *player = malloc(sizeof(struct game_obj));
    player->on_collision = NULL;
    player->coll_type = COLL_DYNAMIC;
    player->tag = OBJ_PLAYER;
    player->pos.x = 0;
    player->pos.y = 0;

    player->size.x = 20;
    player->size.y = 20;

    player->speed = 0.5;

    state->objects[state->obj_amount++] = player;
}

void player_move(struct game_obj *player, int delta_time) {
    player->velocity.x = get_input_horizontal();
    player->velocity.y = get_input_vertical();
    player->velocity = vector_normalize(player->velocity);
    player->velocity = vector_multiply(player->velocity,
                                       player->speed * delta_time);
    player->pos = vector_add(player->pos, player->velocity);
}

void player_shoot(struct game_state *state, struct game_obj player,
                  struct game_obj camera) {
    struct vector bullet_dir = get_mouse_direction(player.pos, camera);
    init_bullet(state, player.pos, bullet_dir, 0.5);
}

void player_update(struct game_state *state, struct game_obj *player,
                   struct game_obj camera, struct inputs inputs, int delta_time) {
    player_move(player, delta_time);
    if (inputs.lmb)
        player_shoot(state, *player, camera);
}

