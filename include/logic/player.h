#ifndef _PLAYER_H
#define _PLAYER_H

#include "engine/engine.h"
#include <SDL3/SDL_events.h>

void init_player(struct game_state *state);

void player_update(struct game_state *state, entity_id_t self, int delta_time);

#endif

