#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include "engine/engine.h"
#include "engine/physics.h"
#include "engine/particle.h"
#include "logic/logic.h"
#include "krft/run_every.h"
#include "render.h"

#define FPS_CAP 60
#define MIN_FRAME_DURATION_MS 1000 / FPS_CAP

static struct game_state local_state;
static struct game_obj *player;
static struct game_obj *camera;

struct update_args {
    SDL_Event *event;
    SDL_Renderer *renderer;
};
bool update(int delta_time, void *update_args_p);

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    // if (argc != 3) {
    //     ERRORF("Usage: %s <server ip/name> <port>", argv[0]);
    //     return 1;
    // }

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer("Nukers", 640, 480, 0, &window, &renderer);
    SDL_Event event;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    init_game(&local_state);
    player = find_obj_by_tag(local_state, OBJ_PLAYER);
    camera = find_obj_by_tag(local_state, OBJ_CAMERA);

    struct update_args update_args = {
        .event = &event,
        .renderer = renderer
    };
    struct run_every_args re_update_args = {
        .func = update,
        .args = &update_args,
        .interval_ms = MIN_FRAME_DURATION_MS,
    };
    run_every(re_update_args);

    return 0;
}

bool update(int delta_time, void *update_args_p) {
    struct update_args *args = update_args_p;
    SDL_Event *event = args->event;
    SDL_Renderer *renderer = args->renderer;
    local_state.inputs.lmb = false;
    local_state.inputs.mmb = false;
    local_state.inputs.rmb = false;
    while (SDL_PollEvent(event)) {
        switch (event->type) {
        case SDL_EVENT_QUIT:
            return false;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            switch (event->button.button) {
            case SDL_BUTTON_LEFT:
                local_state.inputs.lmb = true;
                break;
            case SDL_BUTTON_MIDDLE:
                local_state.inputs.mmb = true;
                break;
            case SDL_BUTTON_RIGHT:
                local_state.inputs.rmb = true;
                break;
            }
            break;
        }
    }

    physics_step(&local_state, delta_time);
    camera->pos = player->pos;

    particle_step(&local_state, delta_time);

    draw_state(renderer, local_state);

    return true;
}

