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
struct game_obj;
struct collision;

typedef int entity_id_t;

struct collision {
	entity_id_t obj1;
	entity_id_t obj2;
	struct vector normal1;
	struct vector normal2;
	float toi;
};

#define SDARRAY_T struct collision
#define SDARRAY_PREFIX collision
#include "krft/sdarray.h"

struct transform {
	struct vector pos;
	entity_id_t entity_id;
};

#define SDARRAY_T struct transform
#define SDARRAY_PREFIX transform
#include "krft/sdarray.h"

struct linear_movement {
	struct vector velocity;
	entity_id_t entity_id;
};

#define SDARRAY_T struct linear_movement
#define SDARRAY_PREFIX linear_movement
#include "krft/sdarray.h"

struct aabb_sprite {
	struct vector size;
	SDL_Color color;
	entity_id_t entity_id;
};

#define SDARRAY_T struct aabb_sprite
#define SDARRAY_PREFIX aabb_sprite
#include "krft/sdarray.h"

struct aabb_collider {
	struct vector size;
	entity_id_t entity_id;
};

#define SDARRAY_T struct aabb_collider
#define SDARRAY_PREFIX aabb_collider
#include "krft/sdarray.h"

struct temporary {
	size_t time_left;
	entity_id_t entity_id;
};

#define SDARRAY_T struct temporary
#define SDARRAY_PREFIX temporary
#include "krft/sdarray.h"

// TODO: this one might need to go into game-specific logic, not engine
struct game_state {
	struct inputs inputs;
	transform_sdarray transforms;
	linear_movement_sdarray linear_movables;
	aabb_collider_sdarray static_colliders;
	aabb_collider_sdarray dynamic_colliders;
	aabb_sprite_sdarray aabb_renderables;
	collision_sdarray collisions;
	temporary_sdarray temporaries;
	bool running;
};

struct AABB_bounds {
	float up;
	float down;
	float left;
	float right;
};

struct AABB_bounds AABB_get_bounds(struct vector pos, struct vector size);

#endif
