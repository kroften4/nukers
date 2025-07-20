#include "logic/logic.h"
#include "engine/engine.h"
#include "engine/game_obj.h"
#include "engine/particle.h"
#include "krft/log.h"
#include "engine/vector.h"
#include <stddef.h>
#include <stdlib.h>
#include "logic/bullet.h"

static void bullet_on_collision(struct game_state *state, struct game_obj *bullet, struct coll_info collision) {
    // TODO: if 2 collisions happen in the same tick, this gets run twice.
    // maybe add on_collision_last method?
    if (collision.other.tag != OBJ_WALL)
        return;

    for (int i = 0; i < 25; i++) {
        struct vector pos = {bullet->pos.x + rand() % 100 - 50, bullet->pos.y + rand() % 100 - 50};
        float scale = rand() % 20 - 10;
        struct vector size = {scale, scale};
        int lifetime = rand() % 300 + 50;
	struct particle explosion_particle = { .type = PAR_EXPLOSION,
					       .lifetime = lifetime,
					       .size = size,
					       .pos = pos,
					       .velocity = VEC_ZERO };
	add_particle(state, explosion_particle);
    }
    object_destroy(state, bullet);
}

void init_bullet(struct game_state *state, struct vector start_pos,
                 struct vector angle, float speed) {
    struct game_obj *bullet = malloc(sizeof(struct game_obj));
    bullet->on_physics_tick = NULL;
    bullet->on_collision = bullet_on_collision;
    bullet->coll_type = COLL_DYNAMIC;
    bullet->tag = OBJ_BULLET;
    bullet->pos = start_pos;
    bullet->speed = speed;
    angle = vector_normalize(angle);
    bullet->velocity.x = angle.x * speed;
    bullet->velocity.y = angle.y * speed;
    bullet->size.x = 5;
    bullet->size.y = 5;
    LOGF("bullet pos(%.2f %.2f) vel(%.2f %.2f)", bullet->pos.x, bullet->pos.y, bullet->velocity.x, bullet->velocity.y);

    state->objects[state->obj_amount++] = bullet;
}

