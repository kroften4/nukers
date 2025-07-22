#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include "engine/input.h"
#include "engine/engine.h"
#include "engine/vector.h"
#include "render.h"

struct inputs inputs;

short get_input_vertical(void)
{
	const bool *keyboard = SDL_GetKeyboardState(NULL);
	bool up = keyboard[SDL_SCANCODE_UP] || keyboard[SDL_SCANCODE_W];
	bool down = keyboard[SDL_SCANCODE_DOWN] || keyboard[SDL_SCANCODE_S];
	return up - down;
}

short get_input_horizontal(void)
{
	const bool *keyboard = SDL_GetKeyboardState(NULL);
	bool left = keyboard[SDL_SCANCODE_LEFT] || keyboard[SDL_SCANCODE_A];
	bool right = keyboard[SDL_SCANCODE_RIGHT] || keyboard[SDL_SCANCODE_D];
	return right - left;
}

struct vector get_mouse_direction(struct vector pos, struct game_obj camera)
{
	struct vector mouse_pos = { 0, 0 };
	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
	mouse_pos = screen_to_world_coords(mouse_pos, camera);
	return vector_subtract(mouse_pos, pos);
}

static void handle_mouse_buttons(struct game_state *state, SDL_Event *event)
{
	switch (event->button.button) {
	case SDL_BUTTON_LEFT:
		state->inputs.lmb = true;
		break;
	case SDL_BUTTON_MIDDLE:
		state->inputs.mmb = true;
		break;
	case SDL_BUTTON_RIGHT:
		state->inputs.rmb = true;
		break;
	}
}

void handle_input(struct game_state *state, SDL_Event *event)
{
	state->inputs.lmb = false;
	state->inputs.mmb = false;
	state->inputs.rmb = false;
	while (SDL_PollEvent(event)) {
		switch (event->type) {
		case SDL_EVENT_QUIT:
			state->running = false;
			return;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			handle_mouse_buttons(state, event);
			break;
		}
	}
}
