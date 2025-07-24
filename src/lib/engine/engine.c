#include "engine/engine.h"
#include <stdbool.h>

void *get_component(struct game_state *state, entity_id_t entity,
		    enum component type)
{
	switch (type) {
	case COMP_TRANSFORM:
		return transform_sdarray_get(&state->transforms, entity);
	case COMP_VELOCITY:
		return velocity_sdarray_get(&state->velocities, entity);
	case COMP_COLLIDER:
		return aabb_collider_sdarray_get(&state->colliders, entity);
	case COMP_AABB_SPRITE:
		return aabb_sprite_sdarray_get(&state->aabb_sprites, entity);
	case COMP_TEMPORARY:
		return temporary_sdarray_get(&state->temporaries, entity);
	}
}

bool has_component(struct game_state *state, entity_id_t entity,
		   enum component type)
{
	switch (type) {
	case COMP_TRANSFORM:
		return state->transforms.sparse[entity] != (size_t)(-1);
	case COMP_VELOCITY:
		return state->velocities.sparse[entity] != (size_t)(-1);
	case COMP_COLLIDER:
		return state->colliders.sparse[entity] != (size_t)(-1);
	case COMP_AABB_SPRITE:
		return state->aabb_sprites.sparse[entity] != (size_t)(-1);
	case COMP_TEMPORARY:
		return state->temporaries.sparse[entity] != (size_t)(-1);
	}
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
