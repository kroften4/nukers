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
	struct velocity *vel = get_component(state, self, COMP_VELOCITY);
	if (vector_dot(vel->v, normal) < 0) {
		// rotate 90
		struct vector surface = { -normal.y, normal.x };
		// project
		float dot = vector_dot(vel->v, surface);
		vel->v = vector_multiply(surface, dot);
	}
}

static void player_input_respond(struct game_state *state, entity_id_t self)
{
	struct velocity *vel = get_component(state, self, COMP_VELOCITY);
	vel->v.x = get_input_horizontal();
	vel->v.y = get_input_vertical();
	vel->v = vector_normalize(vel->v);
	vel->v = vector_multiply(vel->v, global.player.speed);
}

static void player_shoot(struct game_state *state, entity_id_t self)
{
	struct transform *player_pos =
		get_component(state, self, COMP_TRANSFORM);
	struct vector bullet_dir =
		get_mouse_direction(state, player_pos->pos, global.camera);
	init_bullet(state, player_pos->pos, bullet_dir, 0.5);
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
	struct transform transform = { .pos = { 200, 200 } };
	add_component(state, player, COMP_TRANSFORM, &transform);
	global.player.id = player;
	struct vector size = { 20, 20 };
	struct aabb_sprite sprite = { .size = size, .color = cs_orange };
	add_component(state, player, COMP_AABB_SPRITE, &sprite);
	struct aabb_collider collider = { .size = size,
					  .on_collision = player_on_collision,
					  .type = COLL_DYNAMIC };
	add_component(state, player, COMP_COLLIDER, &collider);
	struct velocity vel = { .v = { 0, 0 },
				.on_physics = player_update,
				.on_physics_end = NULL };
	add_component(state, player, COMP_VELOCITY, &vel);
	global.player.speed = 0.25;
}
