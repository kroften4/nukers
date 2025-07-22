#include "engine/input.h"
#include "engine/engine.h"
#include "engine/physics.h"
#include "engine/particle.h"
#include "render.h"
#include <SDL3/SDL.h>

void update(struct game_state *state, int delta_time, SDL_Event *event, SDL_Renderer *renderer)
{
	handle_input(state, event);

	physics_step(state, delta_time);

	particle_step(state, delta_time);

	draw_state(renderer, *state);

	return;
}

void fixed_update()
{
}
