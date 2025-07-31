#include "engine/vector.h"
#include <SDL3/SDL.h>
#include "engine/engine.h"
#include <SDL3/SDL_render.h>
#include "render.h"

static void clear_screen(SDL_Renderer *renderer, SDL_Color bg_color)
{
	SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b,
			       bg_color.a);
	SDL_RenderClear(renderer);
}

struct vector world_to_screen_coords(struct game_state *state,
				     struct vector coords, entity_id_t camera)
{
	struct vector *cam_pos =
		get_component(state, camera, COMP_POSITION);
	struct aabb_sprite *cam_size =
		get_component(state, camera, COMP_AABB_SPRITE);
	struct AABB_bounds cam_bounds =
		AABB_get_bounds(*cam_pos, cam_size->size);
	coords.x -= cam_bounds.left;
	coords.y = cam_bounds.up - coords.y;
	return coords;
}

struct vector screen_to_world_coords(struct game_state *state,
				     struct vector coords, entity_id_t camera)
{
	struct vector *cam_pos =
		get_component(state, camera, COMP_POSITION);
	struct aabb_sprite *cam_size =
		get_component(state, camera, COMP_AABB_SPRITE);
	struct AABB_bounds cam_bounds =
		AABB_get_bounds(*cam_pos, cam_size->size);
	coords.x += cam_bounds.left;
	coords.y = cam_bounds.up - coords.y;
	return coords;
}

static void draw_AABB(struct game_state *state, SDL_Renderer *renderer,
		      entity_id_t aabb, entity_id_t camera)
{
	struct aabb_sprite *aabb_sprite =
		get_component(state, aabb, COMP_AABB_SPRITE);
	SDL_SetRenderDrawColor(renderer, aabb_sprite->color.r,
			       aabb_sprite->color.g, aabb_sprite->color.b,
			       aabb_sprite->color.a);

	struct vector *aabb_pos = get_component(state, aabb, COMP_POSITION);
	struct AABB_bounds bounds =
		AABB_get_bounds(*aabb_pos, aabb_sprite->size);
	struct vector up_left = { .x = bounds.left, .y = bounds.up };

	up_left = world_to_screen_coords(state, up_left, camera);
	SDL_FRect rect = { .x = up_left.x,
			   .y = up_left.y,
			   .w = aabb_sprite->size.x,
			   .h = aabb_sprite->size.y };
	SDL_RenderRect(renderer, &rect);
}

static void draw_all_AABBs(struct game_state *state, SDL_Renderer *renderer,
			   entity_id_t camera)
{
	for (size_t e = 0; e < state->aabb_sprites.size_sparse; e++) {
		if (!has_component(state, e, COMP_AABB_SPRITE))
			continue;
		draw_AABB(state, renderer, e, camera);
	}
}

void draw_state(SDL_Renderer *renderer, struct game_state *state,
		entity_id_t camera)
{
	SDL_Color bg_color = { 255, 255, 255, 255 };
	clear_screen(renderer, bg_color);

	draw_all_AABBs(state, renderer, camera);

	SDL_RenderPresent(renderer);
}
