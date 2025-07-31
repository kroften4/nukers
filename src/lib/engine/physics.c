#include "engine/physics.h"
#include "engine/vector.h"
#include <stdbool.h>
#include <stddef.h>
#include "engine/engine.h"
#include "krft/log.h"

static bool is_valid_toi(float toi)
{
	return 0.0f < toi && toi <= 1.0f;
}

static bool AABB_y_overlaps_at_toi(struct AABB_bounds b1, struct AABB_bounds b2,
				   struct vector relative_vel, float toi)
{
	b1.up += toi * relative_vel.y;
	b1.down += toi * relative_vel.y;
	return b1.down < b2.up && b1.up > b2.down;
}

static bool AABB_x_overlaps_at_toi(struct AABB_bounds b1, struct AABB_bounds b2,
				   struct vector relative_vel, float toi)
{
	b1.left += toi * relative_vel.x;
	b1.right += toi * relative_vel.x;
	return b1.left < b2.right && b1.right > b2.left;
}

static void check_AABB_collision(struct game_state *state, entity_id_t obj1,
				 entity_id_t obj2, float *result_toi,
				 struct vector *normal1, int delta_time)
{
	struct vector vel1 = { 0, 0 };
	if (has_component(state, obj1, COMP_VELOCITY)) {
		struct vector *vel_comp = (struct vector *)get_component(state, obj1, COMP_VELOCITY);
		vel1 = *vel_comp;
	}
	struct vector vel2 = { 0, 0 };
	if (has_component(state, obj2, COMP_VELOCITY)){
		struct vector *vel_comp = (struct vector *)get_component(state, obj2, COMP_VELOCITY);
		vel2 = *vel_comp;
	}
	struct vector relative_vel = vector_subtract(vel1, vel2);
	relative_vel = vector_multiply(relative_vel, delta_time);
	struct vector pos1 =
		*vec_sdarray_get(&state->positions, obj1);
	struct vector pos2 =
		*vec_sdarray_get(&state->positions, obj2);
	struct aabb_collider size1 =
		*aabb_collider_sdarray_get(&state->colliders, obj1);
	struct aabb_collider size2 =
		*aabb_collider_sdarray_get(&state->colliders, obj2);
	struct AABB_bounds obj1_bounds = AABB_get_bounds(pos1, size1.size);
	struct AABB_bounds obj2_bounds = AABB_get_bounds(pos2, size2.size);

	float toi_min = 99;
	*normal1 = VEC_ZERO;

	float toi_right =
		(obj2_bounds.right - obj1_bounds.left) / relative_vel.x;
	bool is_overlapping = AABB_y_overlaps_at_toi(obj1_bounds, obj2_bounds,
						     relative_vel, toi_right);
	if (toi_right < toi_min && is_overlapping)
		toi_min = toi_right;

	float toi_left =
		(obj2_bounds.left - obj1_bounds.right) / relative_vel.x;
	is_overlapping = AABB_y_overlaps_at_toi(obj1_bounds, obj2_bounds,
						relative_vel, toi_left);
	if (toi_left < toi_min && is_overlapping)
		toi_min = toi_left;

	float toi_top = (obj2_bounds.up - obj1_bounds.down) / relative_vel.y;
	is_overlapping = AABB_x_overlaps_at_toi(obj1_bounds, obj2_bounds,
						relative_vel, toi_top);
	if (toi_top < toi_min && is_overlapping)
		toi_min = toi_top;

	float toi_bottom = (obj2_bounds.down - obj1_bounds.up) / relative_vel.y;
	is_overlapping = AABB_x_overlaps_at_toi(obj1_bounds, obj2_bounds,
						relative_vel, toi_bottom);
	if (toi_bottom < toi_min && is_overlapping)
		toi_min = toi_bottom;

	if (toi_min == toi_top)
		*normal1 = vector_add(*normal1, VEC_UP);
	if (toi_min == toi_bottom)
		*normal1 = vector_add(*normal1, VEC_DOWN);
	if (toi_min == toi_left)
		*normal1 = vector_add(*normal1, VEC_LEFT);
	if (toi_min == toi_right)
		*normal1 = vector_add(*normal1, VEC_RIGHT);

	*result_toi = toi_min;
}

static void add_collision(struct game_state *state, float toi, entity_id_t obj1,
			  entity_id_t obj2, struct vector normal1)
{
	struct collision collision = { .toi = toi,
				       .normal1 = normal1,
				       .normal2 = vector_multiply(normal1, -1),
				       .obj1 = obj1,
				       .obj2 = obj2 };
	collision_darray_push(&state->collisions, collision);
}

static void objects_move(struct game_state *state, int delta_time, float toi)
{
	for (size_t e = 0; e < state->velocities.size_sparse; e++) {
		if (state->velocities.sparse[e] == (size_t)-1)
			continue;
		struct vector pos =
			*vec_sdarray_get(&state->positions, e);
		struct vector vel =
			*vec_sdarray_get(&state->velocities, e);
		struct vector corrected_vel =
			vector_multiply(vel, delta_time * toi);
		struct vector new_pos = vector_add(pos, corrected_vel);
		vec_sdarray_set(&state->positions, e, new_pos);
	}
}

void resolve_collision(struct game_state *state, struct collision collision)
{
	struct aabb_collider collider1 =
		*aabb_collider_sdarray_get(&state->colliders, collision.obj1);
	if (collider1.on_collision != NULL)
		collider1.on_collision(state, collision.normal1, collision.obj1,
				       collision.obj2);
	struct aabb_collider collider2 =
		*aabb_collider_sdarray_get(&state->colliders, collision.obj2);
	if (collider2.on_collision != NULL)
		collider2.on_collision(state, collision.normal2, collision.obj2,
				       collision.obj1);
}

static float calc_first_toi_collisions(struct game_state *state, int delta_time)
{
	float toi_min = 99;
	state->collisions.size = 0;

	for (size_t i = 0; i < state->colliders.size_sparse - 1; i++) {
		if (state->colliders.sparse[i] == (size_t)-1)
			continue;
		for (size_t j = i + 1; j < state->colliders.size_sparse; j++) {
			if (state->colliders.sparse[j] == (size_t)-1)
				continue;
			entity_id_t obj1 = i;
			entity_id_t obj2 = j;
			struct aabb_collider collider1 =
				*aabb_collider_sdarray_get(&state->colliders,
							   obj1);
			struct aabb_collider collider2 =
				*aabb_collider_sdarray_get(&state->colliders,
							   obj2);
			if (collider1.type == COLL_STATIC &&
			    collider2.type == COLL_STATIC)
				continue;

			float toi;
			struct vector normal;
			check_AABB_collision(state, obj1, obj2, &toi, &normal,
					     delta_time);
			if (toi == 0) {
				struct collision coll = {
					.toi = toi,
					.normal1 = normal,
					.normal2 = vector_multiply(normal, -1),
					.obj1 = obj1,
					.obj2 = obj2
				};
				resolve_collision(state, coll);
			}
			if (!is_valid_toi(toi))
				continue;

			if (toi == toi_min)
				add_collision(state, toi, obj1, obj2, normal);
			if (toi < toi_min) {
				state->collisions.size = 0;
				add_collision(state, toi, obj1, obj2, normal);
				toi_min = toi;
			}
		}
	}
	if (!is_valid_toi(toi_min))
		return 1;

	return toi_min;
}

void physics_step(struct game_state *state, int delta_time)
{
	int cap = 100;
	int iterations = 0;
	float passed_time = 0;
	while (passed_time < delta_time && iterations < cap) {
		float remaining_time = delta_time - passed_time;
		float toi = calc_first_toi_collisions(state, remaining_time);
		objects_move(state, remaining_time, toi);
		for (size_t i = 0; i < state->collisions.size; i++) {
			struct collision coll = state->collisions.array[i];
			resolve_collision(state, coll);
		}
		passed_time += remaining_time * toi;
		iterations++;
	}
	if (iterations == cap)
		ERROR("reached 100 collision iterations");
}
