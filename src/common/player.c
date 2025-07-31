#include "logic/bullet.h"
#include "engine/input.h"
#include "logic/player.h"
#include "engine/engine.h"
#include "engine/vector.h"
#include "global.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_pixels.h>

static void player_on_collision(struct game_state *state, struct vector normal,
				entity_id_t self, entity_id_t other)
{
	(void)other;
	struct vector *vel = get_component(state, self, COMP_VELOCITY);
	if (vector_dot(*vel, normal) < 0) {
		// rotate 90
		struct vector surface = { -normal.y, normal.x };
		// project
		float dot = vector_dot(*vel, surface);
		*vel = vector_multiply(surface, dot);
	}
}

static void player_input_respond(struct game_state *state, entity_id_t self)
{
	struct vector *vel = get_component(state, self, COMP_VELOCITY);
	vel->x = get_input_horizontal();
	vel->y = get_input_vertical();
	*vel = vector_normalize(*vel);
	*vel = vector_multiply(*vel, global.player.speed);
}

static void player_shoot(struct game_state *state, entity_id_t self)
{
	struct vector player_pos =
		*(struct vector *)get_component(state, self, COMP_POSITION);
	struct vector bullet_dir =
		get_mouse_direction(state, player_pos, global.camera);
	init_bullet(state, player_pos, bullet_dir, 0.75);
}

void player_update(struct game_state *state, entity_id_t self, int delta_time)
{
	(void)delta_time;
	player_input_respond(state, self);
	if (state->inputs.lmb)
		player_shoot(state, self);
}

void init_player(struct game_state *state)
{
	entity_id_t player = add_entity(state);
	struct vector pos = { 200, 200 };
	add_component(state, player, COMP_POSITION, &pos);
	global.player.id = player;
	struct vector size = { 20, 20 };
	struct aabb_sprite sprite = { .size = size, .color = cs_orange };
	add_component(state, player, COMP_AABB_SPRITE, &sprite);
	struct aabb_collider collider = { .size = size,
					  .on_collision = player_on_collision,
					  .type = COLL_DYNAMIC };
	add_component(state, player, COMP_COLLIDER, &collider);
	struct vector vel = { 0, 0 };
	add_component(state, player, COMP_VELOCITY, &vel);
	global.player.speed = 0.5;
}
