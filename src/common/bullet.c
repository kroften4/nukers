#include "logic.h"
#include "krft/engine.h"
#include "krft/log.h"
#include "logic.h"
#include "krft/vector.h"
#include "logic.h"
#include <stddef.h>
#include <stdlib.h>
#include "bullet.h"

void init_explosion_particle(struct game_state *state, enum particle_type type, struct vector pos, struct vector velocity, struct vector size, int lifetime) {
    struct particle *particle = malloc(sizeof(struct particle));

    particle->type = type;
    particle->pos = pos;
    particle->size = size;
    particle->velocity = velocity;
    particle->lifetime = lifetime;

    state->particles[state->particle_amount++] = particle;
}

void bullet_on_collision(struct game_state *state, struct game_obj *bullet, struct coll_info collision) {
    // TODO: if 2 collisions happen in the same tick, this gets run twice.
    // maybe add on_collision_last method?
    if (collision.other.tag != OBJ_WALL)
        return;

    for (int i = 0; i < 25; i++) {
        struct vector pos = {bullet->pos.x + rand() % 100 - 50, bullet->pos.y + rand() % 100 - 50};
        float scale = rand() % 20 - 10;
        struct vector size = {scale, scale};
        int lifetime = rand() % 300 + 50;
        init_explosion_particle(state, PAR_EXPLOSION, pos, VEC_ZERO, size, lifetime);
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

