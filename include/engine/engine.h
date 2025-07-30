#ifndef _ENGINE_H
#define _ENGINE_H

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

typedef int entity_id_t;

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

struct transform {
	struct vector pos;
};

#define SDARRAY_T struct transform
#define SDARRAY_PREFIX transform
#include "krft/sdarray.h"

typedef void (*on_physics_t)(struct game_state *state, entity_id_t self,
			     int delta_time);

struct velocity {
	struct vector v;
	on_physics_t on_physics;
	on_physics_t on_physics_end;
};

#define SDARRAY_T struct velocity
#define SDARRAY_PREFIX velocity
#include "krft/sdarray.h"

struct aabb_sprite {
	struct vector size;
	SDL_Color color;
};

#define SDARRAY_T struct aabb_sprite
#define SDARRAY_PREFIX aabb_sprite
#include "krft/sdarray.h"

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

#define SDARRAY_T struct aabb_collider
#define SDARRAY_PREFIX aabb_collider
#include "krft/sdarray.h"

struct temporary {
	int time_left;
};

#define SDARRAY_T struct temporary
#define SDARRAY_PREFIX temporary
#include "krft/sdarray.h"

#define DARRAY_T entity_id_t
#define DARRAY_PREFIX entity
#include "krft/darray.h"

// TODO: this one might need to go into game-specific logic, not engine
struct game_state {
	struct inputs inputs;
	transform_sdarray transforms;
	velocity_sdarray velocities;
	aabb_collider_sdarray colliders;
	aabb_sprite_sdarray aabb_sprites;
	temporary_sdarray temporaries;
	collision_darray collisions;
	entity_darray removed;
	bool running;
};

enum component {
	COMP_TRANSFORM,
	COMP_VELOCITY,
	COMP_COLLIDER,
	COMP_AABB_SPRITE,
	COMP_TEMPORARY
};

void mark_to_remove(struct game_state *state, entity_id_t entity);

void remove_marked(struct game_state *state);

void *get_component(struct game_state *state, entity_id_t entity,
		    enum component type);

bool has_component(struct game_state *state, entity_id_t entity,
		   enum component type);

struct AABB_bounds {
	float up;
	float down;
	float left;
	float right;
};

entity_id_t add_entity(struct game_state *state);

void remove_entity(struct game_state *state, entity_id_t entity);

void add_component(struct game_state *state, entity_id_t entity,
		   enum component type, void *component_data);

struct AABB_bounds AABB_get_bounds(struct vector pos, struct vector size);

#endif
