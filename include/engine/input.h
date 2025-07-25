#ifndef _INPUT_H
#define _INPUT_H

#include "engine/vector.h"
#include "engine/engine.h"
#include <SDL3/SDL_events.h>

enum direction { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };

short get_input_vertical(void);

short get_input_horizontal(void);

struct vector get_mouse_direction(struct game_state *state, struct vector pos,
				  entity_id_t camera);

void handle_input(struct game_state *state, SDL_Event *event);

#endif
