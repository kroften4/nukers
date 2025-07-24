#ifndef _RENDER_H
#define _RENDER_H

#include <SDL3/SDL.h>
#include "engine/engine.h"

struct vector world_to_screen_coords(struct game_state *state,
				     struct vector coords, entity_id_t camera);

struct vector screen_to_world_coords(struct game_state *state,
				     struct vector coords, entity_id_t camera);

void draw_state(SDL_Renderer *renderer, struct game_state *state,
		entity_id_t camera);

#endif
