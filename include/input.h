#ifndef _INPUT_H
#define _INPUT_H

#include "krft/engine.h"
#include "krft/vector.h"
#include <SDL3/SDL_events.h>

enum direction {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
};

short get_input_vertical(void);

short get_input_horizontal(void);

struct vector get_mouse_direction(struct vector pos, struct game_obj camera);

#endif

