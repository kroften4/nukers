#include <SDL3/SDL.h>
#include "krft/engine.h"
#include "logic.h"
#include "render.h"

void clear_screen(SDL_Renderer *renderer, SDL_Color bg_color) {
    SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    SDL_RenderClear(renderer);
}

struct vector world_to_screen_coords(struct vector coords, struct game_obj camera) {
    struct AABB_bounds cam_bounds = AABB_get_bounds(camera.pos, camera.size);
    coords.x -= cam_bounds.left;
    coords.y = cam_bounds.up - coords.y;
    return coords;
}

struct vector screen_to_world_coords(struct vector coords, struct game_obj camera) {
    struct AABB_bounds cam_bounds = AABB_get_bounds(camera.pos, camera.size);
    coords.x += cam_bounds.left;
    coords.y = cam_bounds.up - coords.y;
    return coords;
}

void draw_AABB(SDL_Renderer *renderer, struct vector pos, struct vector size,
                   struct game_obj camera, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    struct AABB_bounds bounds = AABB_get_bounds(pos, size);
    struct vector up_left = {.x = bounds.left, .y = bounds.up};

    up_left = world_to_screen_coords(up_left, camera);
    SDL_FRect rect = {
        .x = up_left.x,
        .y = up_left.y,
        .w = size.x,
        .h = size.y
    };
    SDL_RenderRect(renderer, &rect);
}

void draw_state(SDL_Renderer *renderer, struct game_state state) {
    struct game_obj *camera = find_obj_by_tag(state, OBJ_CAMERA);
    SDL_Color bg_color = {255, 255, 255, 255};
    clear_screen(renderer, bg_color);

    SDL_Color obj_color = {0, 0, 0, 255};
    for (size_t i = 0; i < state.obj_amount; i++) {
        if ((enum game_obj_tag)state.objects[i]->tag == OBJ_CAMERA)
            continue;
        struct game_obj *obj = state.objects[i];
        draw_AABB(renderer, obj->pos, obj->size, *camera, obj_color);
    }

    SDL_Color particle_color = {255, 165, 0, 255};
    for (size_t i = 0; i < state.particle_amount; i++) {
        struct particle *particle = state.particles[i];
        draw_AABB(renderer, particle->pos, particle->size, *camera, particle_color);
    }

    SDL_RenderPresent(renderer);
}
