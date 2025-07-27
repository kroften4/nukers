#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <SDL3/SDL_pixels.h>
#include <engine/engine.h>

extern SDL_Color cs_transparent;
// Pico 8 color scheme https://pico-8.fandom.com/wiki/Palette
extern SDL_Color cs_black;
extern SDL_Color cs_dark_blue;
extern SDL_Color cs_dark_purple;
extern SDL_Color cs_dark_green;
extern SDL_Color cs_brown;
extern SDL_Color cs_dark_gray;
extern SDL_Color cs_light_gray;
extern SDL_Color cs_white;
extern SDL_Color cs_red;
extern SDL_Color cs_orange;
extern SDL_Color cs_yellow;
extern SDL_Color cs_green;
extern SDL_Color cs_blue;
extern SDL_Color cs_lavender;
extern SDL_Color cs_pink;
extern SDL_Color cs_light_peach;

extern struct global {
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
