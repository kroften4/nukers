#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include "bullet.h"
#include "logic.h"
#include "krft/log.h"
#include "krft/run_every.h"
#include "player.h"
#include "render.h"

#define FPS_CAP 1000
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
    player = find_obj_by_type(local_state, OBJ_PLAYER);
    camera = find_obj_by_type(local_state, OBJ_CAMERA);

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
    struct inputs inputs = {.lmb = false};
    while (SDL_PollEvent(event)) {
        switch (event->type) {
        case SDL_EVENT_QUIT:
            return false;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            inputs.lmb = true;
            break;
        }
    }

    SDL_Color bg_color = {255, 255, 255, 255};
    clear_screen(renderer, bg_color);
    SDL_Color player_color = {0, 0, 0, 255};
    for (size_t i = 0; i < local_state.obj_amount; i++) {
        switch ((enum game_obj_type)local_state.objects[i]->type) {
        case OBJ_BULLET:
            bullet_update(local_state.objects[i], delta_time);
            break;
        case OBJ_PLAYER:
            player_update(&local_state, player, *camera, inputs, delta_time);
            break;
        default:
            continue;
        }
    }
    for (size_t i = 0; i < local_state.obj_amount; i++) {
        if ((enum game_obj_type)local_state.objects[i]->type == OBJ_CAMERA)
            continue;
        draw_game_obj(renderer, *local_state.objects[i], *camera, player_color);
    }
    SDL_RenderPresent(renderer);

    return true;
}

