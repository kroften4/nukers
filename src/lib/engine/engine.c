#include "engine/engine.h"
#include "engine/entity_manager.h"
#include <stdbool.h>
#include <stddef.h>

component_id_t position_id;
component_id_t velocity_id;
component_id_t rigidbody_id;
component_id_t aabb_collider_id;
component_id_t aabb_sprite_id;
component_id_t lifetime_id;

void register_engine_components(struct game_state *state)
{
	position_id = register_component(&state->entity_manager,
					 sizeof(struct vector));
	velocity_id = register_component(&state->entity_manager,
					 sizeof(struct vector));
	rigidbody_id = register_component(&state->entity_manager,
					  sizeof(struct rigidbody));
	aabb_collider_id = register_component(&state->entity_manager,
					      sizeof(struct aabb_collider));
	aabb_sprite_id = register_component(&state->entity_manager,
					    sizeof(struct aabb_sprite));
	lifetime_id = register_component(&state->entity_manager, sizeof(int));
}

void mark_to_remove(struct game_state *state, entity_id_t entity)
{
	entity_darray_push(&state->removed, entity);
}

void remove_marked(struct game_state *state)
{
	for (size_t i = 0; i < state->removed.size; i++) {
		remove_entity(&state->entity_manager, state->removed.array[i]);
	}
	state->removed.size = 0;
}

struct AABB_bounds AABB_get_bounds(struct vector pos, struct vector size)
{
	struct AABB_bounds boundaries = { .up = pos.y + size.y / 2,
					  .down = pos.y - size.y / 2,
					  .left = pos.x - size.x / 2,
					  .right = pos.x + size.x / 2 };
	return boundaries;
}

struct vector linear_interpolate(struct vector pos, struct vector pos_next,
				 float toi)
{
	struct vector pos_interpolated = { 0, 0 };
	pos_interpolated.x = toi * (pos_next.x - pos.x) + pos.x;
	pos_interpolated.y = toi * (pos_next.y - pos.y) + pos.y;
	return pos_interpolated;
}
