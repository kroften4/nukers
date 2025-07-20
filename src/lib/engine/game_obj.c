#include "engine/game_obj.h"
#include "engine/engine.h"
#include <stdlib.h>
#include "utility.h"
#include "krft/log.h"

void add_object_to_state(struct game_obj obj, struct game_state *state)
{
	struct game_obj *obj_p = malloc(sizeof(struct game_obj));
	*obj_p = obj;
	state->objects[state->obj_amount++] = obj_p;
}

void object_destroy(struct game_state *state, struct game_obj *object)
{
	for (size_t i = 0; i < state->obj_amount; i++) {
		if (state->objects[i] == object) {
			free(state->objects[i]);
			remove_element_by_index((void **)state->objects,
						&state->obj_amount, i);
			return;
		}
	}
	ERROR("Did not find the object");
}

