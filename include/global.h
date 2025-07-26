#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <SDL3/SDL_pixels.h>
#include <engine/engine.h>

// Pico 8 color scheme https://pico-8.fandom.com/wiki/Palette
SDL_Color cs_black = { 0, 0, 0, 255 };
SDL_Color cs_dark_blue = { 29, 43, 83, 255 };
SDL_Color cs_dark_purple = { 126, 37, 83, 255 };
SDL_Color cs_dark_green = { 0, 135, 81, 255 };
SDL_Color cs_brown = { 171, 82, 54, 255 };
SDL_Color cs_dark_gray = { 95, 87, 79, 255 };
SDL_Color cs_light_gray = { 194, 195, 199, 255 };
SDL_Color cs_white = { 255, 241, 232, 255 };
SDL_Color cs_red = { 255, 0, 77, 255 };
SDL_Color cs_orange = { 255, 163, 0, 255 };
SDL_Color cs_yellow = { 255, 236, 39, 255 };
SDL_Color cs_green = { 0, 228, 54, 255 };
SDL_Color cs_blue = { 41, 173, 255, 255 };
SDL_Color cs_lavender = { 131, 118, 156, 255 };
SDL_Color cs_pink = { 255, 119, 168, 255 };
SDL_Color cs_light_peach = { 255, 204, 170, 255 };

struct global {
	struct {
		entity_id_t id;
		float speed;
	} player;
	entity_id_t camera;
	struct {
		float speed;
	} bullet;
} global;

#endif
