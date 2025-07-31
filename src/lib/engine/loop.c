#include "engine/input.h"
#include "engine/engine.h"
#include "engine/physics.h"
#include "engine/lifetime.h"
#include "global.h"
#include "logic/logic.h"
#include "render.h"
#include <SDL3/SDL.h>

void update(struct game_state *state, int delta_time, SDL_Event *event, SDL_Renderer *renderer)
{
	handle_input(state, event);

	lifetime_tick(state, delta_time);

	before_physics(state, delta_time);
	physics_step(state, delta_time);
	after_physics(state, delta_time);
	
	remove_marked(state);

	draw_state(renderer, state, global.camera);

	return;
}

void fixed_update()
{
}
