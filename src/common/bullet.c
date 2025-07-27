#include "global.h"
#include "logic/bullet.h"
#include "engine/engine.h"
#include "engine/vector.h"
#include <stddef.h>
#include <stdlib.h>

static void init_explosion_particle(struct game_state *state,
				    struct vector center)
{
	struct vector pos = { center.x + rand() % 100 - 50,
			      center.y + rand() % 100 - 50 };
	float scale = rand() % 20 - 10;
	struct vector size = { scale, scale };
	int lifetime = rand() % 300 + 50;
	entity_id_t explosion = add_entity(state);
	struct transform transform = { .pos = pos };
	add_component(state, explosion, COMP_TRANSFORM, &transform);
	struct aabb_sprite sprite = { .size = size, .color = cs_orange };
	add_component(state, explosion, COMP_AABB_SPRITE, &sprite);
	struct temporary temporary = { .time_left = lifetime };
	add_component(state, explosion, COMP_TEMPORARY, &temporary);
}

static void bullet_on_collision(struct game_state *state, struct vector normal,
				entity_id_t self, entity_id_t other)
{
	(void)normal;
	(void)other;

	// TODO: if 2 collisions happen in the same tick, this gets run twice.
	// maybe add on_collision_last method?

	struct transform *bullet_transform =
		get_component(state, self, COMP_TRANSFORM);
	struct vector explosion_center = bullet_transform->pos;
	for (int i = 0; i < 25; i++) {
		init_explosion_particle(state, explosion_center);
	}
	// FIXME: object_destroy(state, bullet);
}

void init_bullet(struct game_state *state, struct vector start_pos,
		 struct vector angle, float speed)
{
	entity_id_t bullet = add_entity(state);
	angle = vector_normalize(angle);
	struct transform transform = { .pos = start_pos };
	add_component(state, bullet, COMP_TRANSFORM, &transform);
	struct velocity velocity = { .v = { angle.x * speed, angle.y * speed },
				     .on_physics = NULL,
				     .on_physics_end = NULL };
	add_component(state, bullet, COMP_VELOCITY, &velocity);
	struct vector size = { 5, 5 };
	struct aabb_collider collider = { .size = size,
					  .on_collision = bullet_on_collision,
					  .type = COLL_DYNAMIC };
	add_component(state, bullet, COMP_COLLIDER, &collider);
	struct aabb_sprite sprite = { .size = size, .color = cs_black };
	add_component(state, bullet, COMP_AABB_SPRITE, &sprite);
}
