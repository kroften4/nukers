#include "engine/physics.h"
#include "engine/entity_manager.h"
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
	if (has_component(&state->entity_manager, obj1, velocity_id)) {
		struct vector *vel_comp = (struct vector *)get_component(
			&state->entity_manager, obj1, velocity_id);
		vel1 = *vel_comp;
	}
	struct vector vel2 = { 0, 0 };
	if (has_component(&state->entity_manager, obj2, velocity_id)) {
		struct vector *vel_comp = (struct vector *)get_component(
			&state->entity_manager, obj2, velocity_id);
		vel2 = *vel_comp;
	}
	struct vector relative_vel = vector_subtract(vel1, vel2);
	relative_vel = vector_multiply(relative_vel, delta_time);
	struct vector *pos1 =
		get_component(&state->entity_manager, obj1, position_id);
	struct vector *pos2 =
		get_component(&state->entity_manager, obj2, position_id);
	struct aabb_collider *size1 =
		get_component(&state->entity_manager, obj1, aabb_collider_id);
	struct aabb_collider *size2 =
		get_component(&state->entity_manager, obj2, aabb_collider_id);
	struct AABB_bounds obj1_bounds = AABB_get_bounds(*pos1, size1->size);
	struct AABB_bounds obj2_bounds = AABB_get_bounds(*pos2, size2->size);

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
	struct component_pool *velocity_pool =
		&state->entity_manager.component_pools[velocity_id];
	struct component_pool *position_pool =
		&state->entity_manager.component_pools[position_id];
	for (size_t dense = 0; dense < velocity_pool->dense_count; dense++) {
		entity_id_t entity_id = velocity_pool->dense_to_sparse[dense];
		struct vector *pos =
			get_component_from_pool(position_pool, entity_id);
		struct vector *vel =
			get_component_from_pool(velocity_pool, entity_id);
		struct vector corrected_vel =
			vector_multiply(*vel, delta_time * toi);
		struct vector new_pos = vector_add(*pos, corrected_vel);
		*pos = new_pos;
	}
}

void resolve_collision(struct game_state *state, struct collision collision)
{
	struct aabb_collider *collider1 = get_component(
		&state->entity_manager, collision.obj1, aabb_collider_id);
	if (collider1->on_collision != NULL)
		collider1->on_collision(state, collision.normal1,
					collision.obj1, collision.obj2);

	struct aabb_collider *collider2 = get_component(
		&state->entity_manager, collision.obj2, aabb_collider_id);
	if (collider2->on_collision != NULL)
		collider2->on_collision(state, collision.normal2,
					collision.obj2, collision.obj1);
}

static float calc_first_toi_collisions(struct game_state *state, int delta_time)
{
	float toi_min = 99;
	state->collisions.size = 0;

	struct component_pool *collider_pool =
		&state->entity_manager.component_pools[aabb_collider_id];
	for (size_t d_i = 0; d_i < collider_pool->dense_count - 1; d_i++) {
		for (size_t d_j = d_i + 1; d_j < collider_pool->dense_count;
		     d_j++) {
			entity_id_t obj1 = collider_pool->dense_to_sparse[d_i];
			entity_id_t obj2 = collider_pool->dense_to_sparse[d_j];
			struct aabb_collider *collider1 =
				get_component_from_pool(collider_pool, obj1);
			struct aabb_collider *collider2 =
				get_component_from_pool(collider_pool, obj2);
			if (collider1->type == COLL_STATIC &&
			    collider2->type == COLL_STATIC)
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

void apply_total_force(struct game_state *state, entity_id_t entity,
		       int delta_time)
{
	struct vector *vel =
		get_component(&state->entity_manager, entity, velocity_id);

	struct rigidbody *rb =
		get_component(&state->entity_manager, entity, rigidbody_id);
	struct vector acceleration =
		vector_multiply(rb->force, rb->inverse_mass);
	*vel = vector_add(*vel, vector_multiply(acceleration, delta_time));
	rb->force = VEC_ZERO;
}

void apply_forces(struct game_state *state, int delta_time)
{
	struct component_pool *rigidbody_pool =
		&state->entity_manager.component_pools[rigidbody_id];
	for (size_t dense = 0; dense < rigidbody_pool->dense_count; dense++) {
		entity_id_t entity = rigidbody_pool->dense_to_sparse[dense];
		apply_total_force(state, entity, delta_time);
	}
}

void add_force(struct game_state *state, entity_id_t entity,
	       struct vector force)
{
	struct rigidbody *rb =
		get_component(&state->entity_manager, entity, rigidbody_id);
	rb->force = vector_add(rb->force, force);
}
