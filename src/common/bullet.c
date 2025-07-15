#include "krft/engine.h"
#include "krft/log.h"
#include "krft/vector.h"
#include "logic.h"
#include <stdlib.h>
#include "bullet.h"

void init_bullet(struct game_state *state, struct vector start_pos,
                 struct vector angle, float speed) {
    struct game_obj *bullet = malloc(sizeof(struct game_obj));
    bullet->type = OBJ_BULLET;
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

void bullet_update(struct game_obj *bullet, int delta_time) {
    *bullet = linear_move(*bullet, delta_time);
}
