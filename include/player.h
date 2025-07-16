#ifndef _PLAYER_H
#define _PLAYER_H

#include "krft/engine.h"
#include <SDL3/SDL_events.h>

void player_respond_to_input(struct game_obj *player, int delta_time);

void player_update(struct game_state *state, struct game_obj *player);

void init_player(struct game_state *state);

#endif

