#include "logic/logic.h"
#include "engine/engine.h"
#include "global.h"
#include "logic/player.h"
#include <stddef.h>
#include <stdlib.h>

static void camera_follow(struct game_state *state, entity_id_t self,
			  int delta_time)
{
	(void)delta_time;
	struct transform *player_tf =
		get_component(state, global.player.id, COMP_TRANSFORM);
	struct transform *camera_tf = get_component(state, self, COMP_TRANSFORM);
	camera_tf->pos = player_tf->pos;
}

static void init_camera(struct game_state *state)
{
	entity_id_t camera = add_entity(state);
	global.camera = camera;
	struct transform transform = { .pos = { 0, 0 } };
	add_component(state, camera, COMP_TRANSFORM, &transform);
	struct velocity velocity = { .v = { 0, 0 },
				     .on_physics = NULL,
				     .on_physics_end = camera_follow };
	add_component(state, camera, COMP_VELOCITY, &velocity);
	struct aabb_sprite aabb_sprite = { .size = { 640, 480 },
					   .color = cs_transparent };
	add_component(state, camera, COMP_AABB_SPRITE, &aabb_sprite);
}

static entity_id_t init_wall_tile(struct game_state *state, size_t x, size_t y)
{
	entity_id_t tile = add_entity(state);
	int scale = 50;
	struct transform transform = { .pos = { x * scale, y * scale } };
	add_component(state, tile, COMP_TRANSFORM, &transform);
	struct vector size = { scale, scale };
	struct aabb_sprite sprite = { .size = size, .color = cs_dark_purple };
	add_component(state, tile, COMP_AABB_SPRITE, &sprite);
	struct aabb_collider collider = { .size = size,
					  .on_collision = NULL,
					  .type = COLL_STATIC };
	add_component(state, tile, COMP_COLLIDER, &collider);
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

void init_game(struct game_state *state)
{
	state->running = true;
	init_camera(state);
	init_player(state);
	init_walls(state);
}
