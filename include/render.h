#ifndef _RENDER_H
#define _RENDER_H

#include <SDL3/SDL.h>
#include "engine/engine.h"

struct vector world_to_screen_coords(struct vector coords, struct game_obj camera);

struct vector screen_to_world_coords(struct vector coords, struct game_obj camera);

void draw_state(SDL_Renderer *renderer, struct game_state state);

#endif

