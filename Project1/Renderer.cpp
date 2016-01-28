#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include "Renderer.h"
#include <SDL_image.h>

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

Renderer::Renderer(int windowWidth, int windowHeight){
	//Create a Window
	win = SDL_CreateWindow("YASP!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, NULL);
	if (win == nullptr)
	{
		SDL_DestroyWindow(win);
	}

	//Create renderer
	ren = SDL_CreateRenderer(win, 0, SDL_RENDERER_ACCELERATED);
	if (ren == nullptr)
	{
		SDL_DestroyWindow(win);
	}

	surface = SDL_CreateRGBSurface(0, windowWidth, windowHeight, 32, 0, 0, 0, 0);
	SCREEN_WIDTH = windowWidth;
	SCREEN_HEIGHT = windowHeight;
}

void Renderer::RenderRect(SDL_Rect* rectangle, int r, int g, int b){
	//SDL_FillRect(surface, rectangle, SDL_MapRGB(surface->format, r, g, b));

	SDL_SetRenderDrawColor(ren, r, g, b, 255);
	//fill up rectangle with color
	SDL_RenderFillRect(ren, rectangle);
}

void Renderer::Begin() {
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
	// clear the screen
	SDL_RenderClear(ren);
}
void Renderer::DrawImage(SDL_Rect *source, SDL_Rect *dest, SDL_Texture *text){
	SDL_RenderCopy(ren, text, source, dest);	//Copy the image to the rendering object.
}
void Renderer::End() {
	//update the screen with rendering operations
	SDL_RenderPresent(ren);
}
