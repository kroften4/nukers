#include "logic/bullet.h"
#include "engine/input.h"
#include "logic/player.h"
#include "engine/engine.h"
#include "engine/vector.h"
#include "logic/logic.h"
#include <SDL3/SDL_events.h>

#define PLAYER_SPEED 0.5

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
	vel->v = vector_multiply(vel->v, PLAYER_SPEED);
}

static void player_shoot(struct game_state *state, entity_id_t self,
			 entity_id_t camera)
{
	struct transform *player_pos =
		get_component(state, self, COMP_TRANSFORM);
	struct vector bullet_dir =
		get_mouse_direction(state, player_pos->pos, camera);
	init_bullet(state, player_pos->pos, bullet_dir, 0.5);
}

void player_update(struct game_state *state, entity_id_t player)
{
	struct game_obj *camera = find_obj_by_tag(*state, OBJ_CAMERA);
	player_input_respond(player);
	if (state->inputs.lmb)
		player_shoot(state, *player, *camera);
}

void init_player(struct game_state *state)
{
	struct game_obj *player = malloc(sizeof(struct game_obj));
	player->on_collision = player_on_collision;
	player->on_physics_tick = player_update;
	player->coll_type = COLL_DYNAMIC;
	player->tag = OBJ_PLAYER;
	player->pos.x = 200;
	player->pos.y = 200;

	player->size.x = 20;
	player->size.y = 20;

	player->speed = 0.5;

	state->objects[state->obj_amount++] = player;
}
