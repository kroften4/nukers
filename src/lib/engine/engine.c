#include "engine/engine.h"
#include "krft/log.h"
#include <stdbool.h>
#include <stddef.h>

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
	return NULL;
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
	return NULL;
}

entity_id_t add_entity(struct game_state *state)
{
	entity_id_t id = transform_sdarray_push_empty(&state->transforms);
	entity_id_t id2 = velocity_sdarray_push_empty(&state->velocities);
	entity_id_t id3 = aabb_collider_sdarray_push_empty(&state->colliders);
	entity_id_t id4 = aabb_sprite_sdarray_push_empty(&state->aabb_sprites);
	entity_id_t id5 = temporary_sdarray_push_empty(&state->temporaries);
	if (id != id2 || id2 != id3 || id3 != id4 || id4 != id5)
		ERROR("WOMP WOMP");
	return id;
}

void mark_to_remove(struct game_state *state, entity_id_t entity)
{
	entity_darray_push(&state->removed, entity);
}

void remove_marked(struct game_state *state)
{
	for (size_t i = 0; i < state->removed.size; i++) {
		remove_entity(state, state->removed.array[i]);
	}
	state->removed.size = 0;
}

void remove_entity(struct game_state *state, entity_id_t entity)
{
	transform_sdarray_remove(&state->transforms, entity);
	velocity_sdarray_remove(&state->velocities, entity);
	aabb_sprite_sdarray_remove(&state->aabb_sprites, entity);
	aabb_collider_sdarray_remove(&state->colliders, entity);
	temporary_sdarray_remove(&state->temporaries, entity);
}

void add_component(struct game_state *state, entity_id_t entity,
		   enum component type, void *component_data)
{
	switch (type) {
	case COMP_TRANSFORM:
		transform_sdarray_set(&state->transforms, entity,
				      *(struct transform *)component_data);
		break;
	case COMP_VELOCITY:
		velocity_sdarray_set(&state->velocities, entity,
				     *(struct velocity *)component_data);
		break;
	case COMP_COLLIDER:
		aabb_collider_sdarray_set(
			&state->colliders, entity,
			*(struct aabb_collider *)component_data);
		break;
	case COMP_AABB_SPRITE:
		aabb_sprite_sdarray_set(&state->aabb_sprites, entity,
					*(struct aabb_sprite *)component_data);
		break;
	case COMP_TEMPORARY:
		temporary_sdarray_set(&state->temporaries, entity,
				      *(struct temporary *)component_data);
		break;
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
