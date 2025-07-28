#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include "engine/engine.h"
#include "engine/loop.h"
#include "logic/logic.h"
#include "krft/run_every.h"

#define FPS_CAP 144
#define MIN_FRAME_DURATION_MS 1000 / FPS_CAP

static struct game_state local_state;

struct update_args {
	SDL_Event *event;
	SDL_Renderer *renderer;
};
bool client_update(int delta_time, void *update_args_p);

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	// if (argc != 3) {
	//     ERRORF("Usage: %s <server ip/name> <port>", argv[0]);
	//     return 1;
	// }

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer("Nukers", 640, 480, 0, &window, &renderer);
	SDL_Event event;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	init_game(&local_state);

	struct update_args update_args = { .event = &event,
					   .renderer = renderer };
	struct run_every_args re_update_args = {
		.func = client_update,
		.args = &update_args,
		.interval_ms = MIN_FRAME_DURATION_MS,
	};
	run_every(re_update_args);

	return 0;
}

bool client_update(int delta_time, void *update_args_p)
{
	struct update_args *args = update_args_p;
	SDL_Event *event = args->event;
	SDL_Renderer *renderer = args->renderer;
	update(&local_state, delta_time, event, renderer);
	return local_state.running;
}
