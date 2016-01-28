#include <iostream>
#ifndef RENDERER
#define RENDERER
#include <SDL.h>

class Renderer {
public:
	Renderer(int windowWidth, int windowHeight);
	void RenderRect(SDL_Rect* rectangle, int r, int g, int b);
	void DrawImage(SDL_Rect *source, SDL_Rect *dest, SDL_Texture *text);
	void Begin();
	void End();

	SDL_Surface* getSurface()	{ return surface; }
	SDL_Renderer* getRender()	{ return ren; }

private:
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Surface* surface;
};
#endif