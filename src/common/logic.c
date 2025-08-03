#include "logic/logic.h"
#include "engine/engine.h"
#include "global.h"
#include "logic/player.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void camera_follow(struct game_state *state, entity_id_t self,
			  int delta_time)
{
	(void)delta_time;
	struct vector *player_pos = get_component(
		&state->entity_manager, global.player.id, position_id);
	struct vector *camera_pos =
		get_component(&state->entity_manager, self, position_id);
	*camera_pos = *player_pos;
}

static void init_camera(struct game_state *state)
{
	entity_id_t camera = add_entity(&state->entity_manager);
	global.camera = camera;
	struct vector pos = { 0, 0 };
	add_component(&state->entity_manager, camera, position_id, &pos);
	struct vector velocity = { 0, 0 };
	add_component(&state->entity_manager, camera, velocity_id, &velocity);
	struct aabb_sprite aabb_sprite = { .size = { 640, 480 },
					   .color = cs_transparent };
	add_component(&state->entity_manager, camera, aabb_sprite_id,
		      &aabb_sprite);
}

static entity_id_t init_wall_tile(struct game_state *state, size_t x, size_t y)
{
	entity_id_t tile = add_entity(&state->entity_manager);
	int scale = 50;
	struct vector pos = { x * scale, y * scale };
	add_component(&state->entity_manager, tile, position_id, &pos);
	struct vector size = { scale, scale };
	struct aabb_sprite sprite = { .size = size, .color = cs_dark_purple };
	add_component(&state->entity_manager, tile, aabb_sprite_id, &sprite);
	struct aabb_collider collider = { .size = size,
					  .on_collision = NULL,
					  .type = COLL_STATIC };
	add_component(&state->entity_manager, tile, aabb_collider_id,
		      &collider);
	return tile;
}

static void init_walls(struct game_state *state)
{
	bool walls[10][10] = { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			       { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
			       { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
			       { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
			       { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
			       { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
			       { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
			       { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
			       { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
			       { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };
	size_t w = 10;
	size_t h = 10;
	entity_id_t wall_grid[w][h];
	for (size_t y = 0; y < h; y++) {
		for (size_t x = 0; x < w; x++) {
			if (walls[y][x])
				wall_grid[y][x] = init_wall_tile(state, x, y);
		}
	}
	// TODO: idk maybe i'll use it for smth
	(void)wall_grid;
}

void before_physics(struct game_state *state, int delta_time)
{
	player_update(state, global.player.id, delta_time);
}

void after_physics(struct game_state *state, int delta_time)
{
	camera_follow(state, global.camera, delta_time);
}

void init_game(struct game_state *state)
{
	memset(state, 0, sizeof(struct game_state));
	register_engine_components(state);
	state->running = true;
	init_camera(state);
	init_player(state);
	init_walls(state);
}
