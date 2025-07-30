#include "engine/engine.h"

void lifetime_tick(struct game_state *state, int delta_time)
{
	for (size_t i = 0; i < state->temporaries.size_dense; i++) {
		struct temporary *temp = &state->temporaries.dense[i];
		temp->time_left -= delta_time;
	}
	for (size_t e = 0; e < state->temporaries.size_sparse; e++) {
		if (!has_component(state, e, COMP_TEMPORARY))
			continue;
		struct temporary *temp =
			get_component(state, e, COMP_TEMPORARY);
		if (temp->time_left <= 0)
			mark_to_remove(state, e);
	}
}
