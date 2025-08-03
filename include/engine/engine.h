#ifndef _ENGINE_H
#define _ENGINE_H

#include "engine/entity_manager.h"
#include "engine/vector.h"
#include <SDL3/SDL_pixels.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct inputs {
	bool lmb;
	bool mmb;
	bool rmb;
};

struct game_state;
struct collision;

struct aabb_sprite {
	struct vector size;
	SDL_Color color;
};

enum collision_type
#if __STDC_VERSION__ >= 202300L
	: bool
#endif
{
	COLL_DYNAMIC,
	COLL_STATIC
};

typedef void (*on_collision_t)(struct game_state *state, struct vector normal,
			       entity_id_t self, entity_id_t other);

struct aabb_collider {
	struct vector size;
	on_collision_t on_collision;
	enum collision_type type;
};

struct rigidbody {
	struct vector force;
	float inverse_mass;
};

#define DARRAY_T entity_id_t
#define DARRAY_PREFIX entity
#include "krft/darray.h"

struct collision {
	entity_id_t obj1;
	entity_id_t obj2;
	struct vector normal1;
	struct vector normal2;
	float toi;
};

#define DARRAY_T struct collision
#define DARRAY_PREFIX collision
#include "krft/darray.h"

// TODO: add systems
struct game_state {
	struct entity_manager entity_manager;
	collision_darray collisions;
	entity_darray removed;
	struct inputs inputs;
	bool running;
};

extern component_id_t position_id;
extern component_id_t velocity_id;
extern component_id_t rigidbody_id;
extern component_id_t aabb_collider_id;
extern component_id_t aabb_sprite_id;
extern component_id_t lifetime_id;

void register_engine_components(struct game_state *state);

void mark_to_remove(struct game_state *state, entity_id_t entity);

void remove_marked(struct game_state *state);

struct AABB_bounds {
	float up;
	float down;
	float left;
	float right;
};

struct AABB_bounds AABB_get_bounds(struct vector pos, struct vector size);

#endif
