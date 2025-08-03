#include "engine/engine.h"
#include "engine/entity_manager.h"
#include <stddef.h>

void lifetime_tick(struct game_state *state, int delta_time)
{
	struct component_pool *lifetime_pool =
		&state->entity_manager.component_pools[lifetime_id];

	for (size_t dense = 0; dense < lifetime_pool->dense_count; dense++) {
		entity_id_t entity_id = lifetime_pool->dense_to_sparse[dense];
		int *const time_left =
			get_component_from_pool(lifetime_pool, entity_id);
		*time_left -= delta_time;
		if (*time_left <= 0)
			mark_to_remove(state, entity_id);
	}
}
