#include "engine/entity_manager.h"
#include "engine/physics.h"
#include "global.h"
#include "logic/bullet.h"
#include "engine/engine.h"
#include "engine/vector.h"
#include <stddef.h>
#include <stdlib.h>

static void init_explosion_particle(struct game_state *state,
				    struct vector center)
{
	entity_id_t explosion = add_entity(&state->entity_manager);

	struct vector pos = { center.x + rand() % 100 - 50,
			      center.y + rand() % 100 - 50 };
	add_component(&state->entity_manager, explosion, position_id, &pos);

	float scale = rand() % 20 - 10;
	struct vector size = { scale, scale };
	struct aabb_sprite sprite = { .size = size, .color = cs_orange };
	add_component(&state->entity_manager, explosion, aabb_sprite_id, &sprite);

	int lifetime = rand() % 300 + 50;
	add_component(&state->entity_manager, explosion, lifetime_id, &lifetime);
}

static void bullet_on_collision(struct game_state *state, struct vector normal,
				entity_id_t self, entity_id_t other)
{
	(void)normal;
	(void)other;
	if (state->entity_manager.component_pools[position_id].sparse[self] == INVALID_INDEX)
		return;

	// TODO: if 2 collisions happen in the same tick, this gets run twice.
	// maybe add on_collision_last method?

	struct vector *bullet_pos = get_component(&state->entity_manager, self, position_id);
	struct vector explosion_center = *bullet_pos;
	for (int i = 0; i < 25; i++) {
		init_explosion_particle(state, explosion_center);
	}
	mark_to_remove(state, self);
}

void init_bullet(struct game_state *state, struct vector start_pos,
		 struct vector angle, float speed)
{
	entity_id_t bullet = add_entity(&state->entity_manager);

	struct vector pos = start_pos;
	add_component(&state->entity_manager, bullet, position_id, &pos);

	angle = vector_normalize(angle);
	struct vector velocity = { angle.x * speed, angle.y * speed };
	add_component(&state->entity_manager, bullet, velocity_id, &velocity);

	struct rigidbody rb = { .force = VEC_ZERO, .inverse_mass = 1 };
	add_component(&state->entity_manager, bullet, rigidbody_id, &rb);

	struct vector size = { 5, 5 };
	struct aabb_collider collider = { .size = size,
					  .on_collision = bullet_on_collision,
					  .type = COLL_DYNAMIC };
	add_component(&state->entity_manager, bullet, aabb_collider_id, &collider);

	struct aabb_sprite sprite = { .size = size, .color = cs_black };
	add_component(&state->entity_manager, bullet, aabb_sprite_id, &sprite);
}

void bullet_on_update(struct game_state *state, entity_id_t self)
{
	struct vector *vel = get_component(&state->entity_manager, self, velocity_id);
	struct vector direction = vector_normalize(*vel);
	struct vector force = vector_multiply(direction, 0.001);
	add_force(state, self, force);
}
