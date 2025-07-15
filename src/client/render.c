#include <SDL3/SDL.h>
#include "krft/engine.h"
#include "render.h"

void clear_screen(SDL_Renderer *renderer, SDL_Color bg_color) {
    SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    SDL_RenderClear(renderer);
}

struct vector world_to_screen_coords(struct vector coords, struct game_obj camera) {
    struct AABB_bounds cam_bounds = AABB_get_bounds(camera);
    coords.x -= cam_bounds.left;
    coords.y = cam_bounds.up - coords.y;
    return coords;
}

struct vector screen_to_world_coords(struct vector coords, struct game_obj camera) {
    struct AABB_bounds cam_bounds = AABB_get_bounds(camera);
    coords.x += cam_bounds.left;
    coords.y = cam_bounds.up - coords.y;
    return coords;
}

void draw_game_obj(SDL_Renderer *renderer, struct game_obj object,
                   struct game_obj camera, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    struct AABB_bounds bounds = AABB_get_bounds(object);
    struct vector up_left = {.x = bounds.left, .y = bounds.up};

    object.pos = world_to_screen_coords(object.pos, camera);
    up_left = world_to_screen_coords(up_left, camera);
    SDL_FRect rect = {
        .x = up_left.x,
        .y = up_left.y,
        .w = object.size.x,
        .h = object.size.y
    };
    SDL_RenderFillRect(renderer, &rect);
}

