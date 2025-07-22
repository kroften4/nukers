#ifndef _ENGINE_H
#define _ENGINE_H

#include "engine/vector.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_OBJECTS_AMOUNT 500
#define MAX_PARTICLES_AMOUNT 2000

struct inputs {
	bool lmb;
	bool mmb;
	bool rmb;
};

enum collision_type
#if __STDC_VERSION__ >= 202300L
	: uint8_t
#endif
{
	COLL_STATIC,
	COLL_DYNAMIC,
	COLL_NONE
};

struct game_state;
struct game_obj;
struct coll_info;

struct game_state {
	struct inputs inputs;
	struct game_obj *objects[MAX_OBJECTS_AMOUNT];
	struct particle *particles[MAX_PARTICLES_AMOUNT];
	struct coll_info *collisions;
	size_t collisions_amount;
	size_t obj_amount;
	size_t particle_amount;
	bool running;
};

struct game_obj {
	int id;
	int tag;
	enum collision_type coll_type;
	float speed;
	struct vector pos;
	struct vector velocity;
	struct vector size;
};

struct coll_info {
	struct game_obj obj1;
	struct game_obj obj2;
	struct vector normal1;
	struct vector normal2;
	float toi;
};

struct AABB_bounds {
	float up;
	float down;
	float left;
	float right;
};

struct AABB_bounds AABB_get_bounds(struct vector pos, struct vector size);

#endif
