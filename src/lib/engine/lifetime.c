#include "engine/engine.h"

void lifetime_tick(struct game_state *state, int delta_time)
{
	for (size_t i = 0; i < state->lifetimes.size_dense; i++) {
		int *const time_left = &state->lifetimes.dense[i];
		*time_left -= delta_time;
	}
	for (size_t e = 0; e < state->lifetimes.size_sparse; e++) {
		if (!has_component(state, e, COMP_LIFETIME))
			continue;
		int *const time_left =
			get_component(state, e, COMP_LIFETIME);
		if (*time_left <= 0)
			mark_to_remove(state, e);
	}
}
