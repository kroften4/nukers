#ifndef _LOOP_H
#define _LOOP_H

#include <SDL3/SDL.h>
#include "engine/engine.h"

void update(struct game_state *state, int delta_time, SDL_Event *event,
	    SDL_Renderer *renderer);

#endif
