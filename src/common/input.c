#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include "input.h"
#include "krft/engine.h"
#include "krft/vector.h"
#include "render.h"

short get_input_vertical(void) {
    const bool *keyboard = SDL_GetKeyboardState(NULL);
    bool up = keyboard[SDL_SCANCODE_UP] || keyboard[SDL_SCANCODE_W];
    bool down = keyboard[SDL_SCANCODE_DOWN] || keyboard[SDL_SCANCODE_S];
    return up - down;
}

short get_input_horizontal(void) {
    const bool *keyboard = SDL_GetKeyboardState(NULL);
    bool left = keyboard[SDL_SCANCODE_LEFT] || keyboard[SDL_SCANCODE_A];
    bool right = keyboard[SDL_SCANCODE_RIGHT] || keyboard[SDL_SCANCODE_D];
    return right - left;
}

struct vector get_mouse_direction(struct vector pos, struct game_obj camera) {
    struct vector mouse_pos = {0, 0};
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
    mouse_pos = screen_to_world_coords(mouse_pos, camera);
    return vector_subtract(mouse_pos, pos);
}

